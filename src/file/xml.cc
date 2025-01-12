#include "file/xml.hh"

#include <libxml/encoding.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>

__attribute__((constructor)) static void initialize() { LIBXML_TEST_VERSION }

__attribute__((destructor)) static void deinitialize() { xmlCleanupParser(); }

XmlDocument::XmlDocument(const std::string& path)
    : path(path)
{
    this->libxml_doc = xmlParseFile(path.c_str());
    if (this->libxml_doc == NULL) {
        fprintf(stderr,
            "XmlDocument::XmlDocument: Error: Failed to parse Xml File '%s': "
            "%s",
            path.c_str(), xmlGetLastError()->message);
        abort();
    }

    this->root_elem = std::make_shared<XmlElement>(
        *this, nullptr, xmlDocGetRootElement(this->libxml_doc));
}

XmlDocument::~XmlDocument() { xmlFreeDoc(this->libxml_doc); }

int XmlDocument::saveDocument(const std::string& path)
{
    return xmlSaveFile((const char*)path.c_str(), this->libxml_doc);
}

XmlElement& XmlDocument::getRootElement() { return *this->root_elem.get(); }

const std::string& XmlDocument::getFilePath() const { return this->path; }

XmlElement::XmlElement(
    XmlDocument& doc, XmlElement* parent, xmlNode* libxml_node)
    : doc(doc)
    , parent(parent)
    , libxml_node(libxml_node)
{
    for (xmlNode* child = this->libxml_node->children; child != NULL;
         child = child->next) {
        switch (child->type) {
        case XML_ELEMENT_NODE:
            children.push_back(
                std::make_shared<XmlElement>(this->doc, this, child));
            break;
        case XML_TEXT_NODE:
            text_nodes.push_back(std::string((const char*)child->content));
            break;
        case XML_PI_NODE:
        case XML_XINCLUDE_START:
        case XML_XINCLUDE_END:
        default:
            break;
        }
    }
}

XmlElement::~XmlElement() { }

unsigned short XmlElement::getXmlPosition() const
{
    return this->libxml_node->line;
}

const char* XmlElement::getName() const
{
    return (const char*)this->libxml_node->name;
}

void XmlElement::setName(const std::string& name)
{
    xmlNodeSetName(this->libxml_node, (const xmlChar*)name.c_str());
}

const std::vector<std::string>& XmlElement::getTextNodes() const
{
    return this->text_nodes;
}

XmlDocument& XmlElement::getDocument() { return this->doc; }
XmlElement* XmlElement::getParent() { return this->parent; }

const std::vector<std::shared_ptr<XmlElement>>&
XmlElement::getChildElements() const
{
    return this->children;
}

const char* XmlElement::getAttribute(const std::string& key) const
{
    return (const char*)xmlGetProp(
        this->libxml_node, (const xmlChar*)key.c_str());
}

bool XmlElement::hasAttribute(const std::string& key) const
{
    return xmlGetProp(this->libxml_node, (const xmlChar*)key.c_str()) != NULL;
}

void XmlElement::setAttribute(const std::string& key, const std::string& value)
{
    xmlSetProp(this->libxml_node, (const xmlChar*)key.c_str(),
        (const xmlChar*)value.c_str());
}
