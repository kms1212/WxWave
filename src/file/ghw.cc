#include "file/ghw.hh"

GhwTraceHandle::GhwTraceHandle(const GhwFile& file, unsigned int sig_index)
    : trace_data(file.getTraceData(sig_index))
    , current_time(0)
    , current_data_index(0)
{
    if (trace_data.size() < 1) {
        fprintf(stderr,
            "GhwTraceHandle::GhwTraceHandle: Error: Trace data cannot be "
            "empty");
        abort();
    }
}

GhwTraceHandle::~GhwTraceHandle() { }

void GhwTraceHandle::setCurrentTime(Time time)
{
    unsigned int trace_data_size = this->trace_data.size();
    unsigned int idx = 0;

    while (idx < trace_data_size - 1 && this->trace_data[idx].time < time) {
        idx++;
    }
    if (this->trace_data[idx].time > time)
        idx--;

    this->current_data_index = idx;
    this->current_time = time;
}

Time GhwTraceHandle::getCurrentTime() const { return this->current_time; }

bool GhwTraceHandle::seekNextTransition()
{
    if (this->current_data_index >= this->trace_data.size() - 1) {
        return false;
    }

    this->current_data_index++;
    this->current_time = this->trace_data[this->current_data_index].time;
    return true;
}

LogicValue GhwTraceHandle::getValue() const
{
    return this->trace_data[this->current_data_index].value;
}

static const char* get_literal(union ghw_type* type, uint32_t e)
{
    if (e >= type->en.nbr) {
        return "?";
    } else {
        return type->en.lits[e];
    }
}

static std::string get_value_str(union ghw_val* val, union ghw_type* type)
{
    switch (ghw_get_base_type(type)->kind) {
    case ghdl_rtik_type_b2:
        return std::string(get_literal(type, val->b2));
    case ghdl_rtik_type_e8:
        return std::string(get_literal(type, val->e8));
    case ghdl_rtik_type_i32:
        return std::to_string(val->i32);
    case ghdl_rtik_type_p64:
        return std::to_string(val->i64);
    case ghdl_rtik_type_f64:
        return std::to_string(val->f64);
    default:
        fprintf(stderr, "error: Unknown type %d\n", type->kind);
        abort();
    }
}

static std::string get_range_str(union ghw_type* type, union ghw_range* rng)
{
    switch (rng->kind) {
    case ghdl_rtik_type_b2:
        return std::string(get_literal(type, rng->b2.left)) + " "
            + ghw_get_dir(rng->b2.dir) + " " + get_literal(type, rng->b2.right);
    case ghdl_rtik_type_e8:
        return std::string(get_literal(type, rng->e8.left)) + " "
            + ghw_get_dir(rng->e8.dir) + " " + get_literal(type, rng->e8.right);
    case ghdl_rtik_type_i32:
    case ghdl_rtik_type_p32:
        return std::to_string(rng->i32.left) + " " + ghw_get_dir(rng->i32.dir)
            + " " + std::to_string(rng->i32.right);
    case ghdl_rtik_type_i64:
    case ghdl_rtik_type_p64:
        return std::to_string(rng->i64.left) + " " + ghw_get_dir(rng->i64.dir)
            + " " + std::to_string(rng->i64.right);
    case ghdl_rtik_type_f64:
        return std::to_string(rng->f64.left) + " " + ghw_get_dir(rng->f64.dir)
            + " " + std::to_string(rng->f64.right);
    default:
        return "?(" + std::to_string(rng->kind) + ")";
    }
}

static std::string get_array_bound_str(struct ghw_subtype_array* a)
{
    std::string str = "(";
    struct ghw_type_array* base
        = (struct ghw_type_array*)ghw_get_base_type(a->base);

    for (unsigned int i = 0; i < base->nbr_dim; i++) {
        if (i)
            str += ", ";
        str += get_range_str(base->dims[i], a->rngs[i]);
    }
    str += ")";
    return str;
}

static std::string get_record_bound_str(struct ghw_subtype_record* r)
{
    std::string str = "(";
    struct ghw_type_record* base = r->base;

    for (unsigned int i = 0; i < base->nbr_fields; i++) {
        if (r->els[i].type != base->els[i].type) {
            if (str.size() != 1)
                str += ", ";
            str += base->els[i].name;
            switch (base->els[i].type->kind) {
            case ghdl_rtik_subtype_array:
                str += get_array_bound_str(&r->els[i].type->sa);
                break;
            case ghdl_rtik_subtype_record:
                str += get_record_bound_str(&r->els[i].type->sr);
                break;
            default:
                str += "??? (" + std::to_string(r->els[i].type->kind) + ")";
            }
        }
    }

    str += ")";
    return str;
}

static std::string get_subtype_indication(
    struct ghw_handler* h, union ghw_type* type)
{
    if (type->common.name == h->str_table[0]) {
        // anonymous subtype
        switch (type->kind) {
        case ghdl_rtik_subtype_scalar: {
            struct ghw_subtype_scalar* s = &type->ss;
            return std::string(s->base->common.name) + " range "
                + get_range_str(s->base, s->rng);
        } break;
        case ghdl_rtik_subtype_array: {
            struct ghw_subtype_array* a = &type->sa;
            return std::string(a->base->common.name) + get_array_bound_str(a);
        } break;
        case ghdl_rtik_subtype_record: {
            struct ghw_subtype_record* r = &type->sr;
            return std::string(r->base->name) + get_record_bound_str(r);
        } break;
        case ghdl_rtik_subtype_unbounded_array:
        case ghdl_rtik_subtype_unbounded_record:
        default:
            fprintf(stderr, "error: Unknown subtype %d\n", type->kind);
            exit(1);
        }
    } else {
        return std::string(type->common.name);
    }
}

GhwFile::GhwFile(const std::string& path)
    : end_time(0)
{
    this->handler.flag_verbose = 1;
    this->openHandle(path);
    this->loadData();
}

GhwFile::~GhwFile() { this->closeHandle(); }

int GhwFile::openHandle(const std::string& path)
{
    return ghw_open(&this->handler, path.c_str());
}

void GhwFile::closeHandle() { ghw_close(&this->handler); }

void GhwFile::loadData()
{
    ghw_read_base(&this->handler);

    trace_data.resize(this->handler.nbr_sigs);

    this->top.reset(new GhwHierarchy(*this, &this->handler, this->handler.hie));

    std::vector<int> last_value(this->handler.nbr_sigs);
    ghw_sm_type sm = ghw_sm_init;
    int eof = 0;
    Time current_time(0);

    while (!eof) {
        switch (ghw_read_sm(&this->handler, &sm)) {
        case ghw_res_snapshot:
        case ghw_res_cycle: {
            current_time = Time(this->handler.snap_time);
            for (int i = 0; i < this->handler.nbr_sigs; i++) {
                struct ghw_sig* s = &this->handler.sigs[i];
                char value;

                if (s->type != NULL) {
                    value = get_value_str(s->val, s->type)[1];

                    if (!trace_data[i].size() || last_value[i] != value) {
                        last_value[i] = value;
                        if (trace_data[i].size()
                            && trace_data[i].back().time == current_time) {
                            trace_data[i].pop_back();
                        }

                        trace_data[i].push_back(
                            GhwTraceData(current_time, LogicValue(value)));
                    }
                }
            }
        } break;
        case ghw_res_eof:
            eof = 1;
            break;
        default:
            abort();
        }
    }

    this->end_time = current_time;

end:
    return;
}

const std::vector<std::string>& GhwFile::getStringTable() const
{
    return this->string_table;
}

const std::vector<std::string>& GhwFile::getTypeTable() const
{
    return this->type_table;
}

const GhwHierarchy& GhwFile::getTop() const { return *this->top; }

const std::vector<GhwTraceData>& GhwFile::getTraceData(
    unsigned int signal) const
{
    return trace_data[signal];
}

Time GhwFile::getEndTime() const { return this->end_time; }

GhwHierarchy::GhwHierarchy(
    const GhwFile& file, struct ghw_handler* h, struct ghw_hie* hie)
    : file(file)
    , h(h)
    , hie(hie)
{
    this->kind = ghw_get_hie_name(hie);
    this->trace_data = nullptr;

    switch (hie->kind) {
    case ghw_hie_design:
        this->name = "top";
    case ghw_hie_block:
    case ghw_hie_generate_if:
    case ghw_hie_generate_for:
    case ghw_hie_instance: {
        if (hie->name) {
            this->name = hie->name;
        }
        if (hie->kind == ghw_hie_generate_for) {
            this->name
                += get_value_str(hie->u.blk.iter_value, hie->u.blk.iter_type);
        }
        struct ghw_hie* child = hie->u.blk.child;

        while (child) {
            switch (child->kind) {
            case ghw_hie_package:
                this->packages.push_back(child->name);
                break;
            case ghw_hie_design:
            case ghw_hie_process:
                break;
            case ghw_hie_block:
            case ghw_hie_generate_if:
            case ghw_hie_generate_for:
            case ghw_hie_instance:
            case ghw_hie_signal:
            case ghw_hie_port_in:
            case ghw_hie_port_out:
            case ghw_hie_port_inout:
            case ghw_hie_port_buffer:
            case ghw_hie_port_linkage:
                children.push_back(GhwHierarchy(this->file, h, child));
                break;
            default:
                abort();
            }

            child = child->brother;
        }
    } break;
    case ghw_hie_signal:
    case ghw_hie_port_in:
    case ghw_hie_port_out:
    case ghw_hie_port_inout:
    case ghw_hie_port_buffer:
    case ghw_hie_port_linkage: {
        unsigned int* sigs = hie->u.sig.sigs;
        unsigned int k = 0, num;

        this->name = hie->name;
        this->subtype = get_subtype_indication(h, hie->u.sig.type).c_str();

        while (sigs[k] != GHW_NO_SIG) {
            this->sig_index_start = this->sig_index_end = sigs[k];
            for (num = 1; sigs[k + num] != GHW_NO_SIG; num++) {
                if (sigs[k + num] != sigs[k + num - 1] + 1)
                    break;
            }
            if (num > 1)
                this->sig_index_end = sigs[k + num - 1];
            k += num;
        }
    } break;
    default:
        abort();
    }

    this->trace_data = &file.getTraceData(sig_index_start);
}

const std::string& GhwHierarchy::getKind() const { return this->kind; }
const std::string& GhwHierarchy::getName() const { return this->name; }
const std::string& GhwHierarchy::getSubtype() const { return this->subtype; }
const std::vector<std::string>& GhwHierarchy::getPackages() const
{
    return this->packages;
}
const std::map<std::string, std::string>& GhwHierarchy::getProperties() const
{
    return this->properties;
}

const std::vector<GhwHierarchy>& GhwHierarchy::getChildren() const
{
    return this->children;
}

bool GhwHierarchy::getSignalIndexRange(
    unsigned int* start, unsigned int* end) const
{
    if (this->sig_index_start >= 0) {
        *start = this->sig_index_start;
        *end = this->sig_index_end;
        return true;
    } else {
        return false;
    }
}
