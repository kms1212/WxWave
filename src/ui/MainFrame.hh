#ifndef __MAINFRAME__
#define __MAINFRAME__

#include "ui/UIBase.h"

#include "file/ghw.hh"

class HierarchyViewModelNode {
private:
    HierarchyViewModelNode* parent;
    std::vector<HierarchyViewModelNode> children;

    const GhwHierarchy& hie;

public:
    HierarchyViewModelNode(
        HierarchyViewModelNode* parent, const GhwHierarchy& hie)
        : parent(parent)
        , hie(hie)
    {
        for (const GhwHierarchy& child : hie.getChildren()) {
            children.push_back(HierarchyViewModelNode(this, child));
        }
    }

    ~HierarchyViewModelNode() = default;

    bool IsContainer() const { return hie.getChildren().size() != 0; }

    HierarchyViewModelNode* GetParent() { return this->parent; }
    std::vector<HierarchyViewModelNode>& GetChildren()
    {
        return this->children;
    }
    unsigned GetChildrenCount() const { return this->children.size(); }

    const GhwHierarchy& GetData() { return this->hie; }
};

class HierarchyViewModel : public wxDataViewModel {
private:
    HierarchyViewModelNode root;

public:
    HierarchyViewModel(const GhwHierarchy& top)
        : root(NULL, top)
    {
    }
    ~HierarchyViewModel() = default;

    virtual void GetValue(wxVariant& variant, const wxDataViewItem& item,
        unsigned int col) const override
    {
        wxASSERT(item.IsOk());

        HierarchyViewModelNode* node = (HierarchyViewModelNode*)item.GetID();

        switch (col) {
        case 0:
            variant = node->GetData().getName();
            break;
        case 1:
            variant = node->GetData().getSubtype();
            break;
        default:
            wxLogError("HierarchyViewModel::GetValue: Unknown column %d", col);
        }
    }

    virtual bool SetValue(const wxVariant& variant, const wxDataViewItem& item,
        unsigned int col) override
    {
        return false;
    }

    virtual wxDataViewItem GetParent(const wxDataViewItem& item) const override
    {
        if (!item.IsOk())
            return wxDataViewItem(0);

        HierarchyViewModelNode* node = (HierarchyViewModelNode*)item.GetID();

        if (node == &root)
            return wxDataViewItem(0);

        return wxDataViewItem((void*)node->GetParent());
    }
    virtual bool IsContainer(const wxDataViewItem& item) const override
    {
        if (!item.IsOk())
            return true;

        HierarchyViewModelNode* node = (HierarchyViewModelNode*)item.GetID();
        return node->IsContainer();
    }
    virtual unsigned int GetChildren(
        const wxDataViewItem& parent, wxDataViewItemArray& array) const override
    {
        HierarchyViewModelNode* node = (HierarchyViewModelNode*)parent.GetID();

        if (!node) {
            array.Add(wxDataViewItem((void*)&this->root));
            return 1;
        }

        if (node->GetChildrenCount() == 0) {
            return 0;
        }

        for (const auto& child : node->GetChildren()) {
            array.Add(wxDataViewItem((void*)&child));
        }

        return array.size();
    }
};

class MainFrame : public MainFrameUI {
    DECLARE_CLASS(MainFrame)
    DECLARE_EVENT_TABLE();

private:
    GhwFile ghw;
    HierarchyViewModel* hie_model;

    virtual void onHierarchyViewSelectionChanged(
        wxDataViewEvent& event) override;
    virtual void onToolBarZoomInClicked(wxCommandEvent& event) override;
    virtual void onToolBarZoomOutClicked(wxCommandEvent& event) override;

public:
    MainFrame(wxWindow* present, wxWindowID id = wxID_ANY);
    ~MainFrame();
};

#endif // __MAINFRAME__
