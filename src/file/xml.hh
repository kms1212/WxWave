#ifndef __XML_HH__
#define __XML_HH__

#include <map>
#include <string>
#include <vector>

#include <libxml/parser.h>

class XmlElement;

class XmlDocument {
private:
    std::string path;
    std::shared_ptr<XmlElement> root_elem;
    xmlDocPtr libxml_doc;

public:
    XmlDocument(const std::string& path);
    ~XmlDocument();

    int saveDocument(const std::string& path);

    XmlElement& getRootElement();
    const std::string& getFilePath() const;
};

class XmlElement {
private:
    XmlDocument& doc;
    XmlElement* parent;
    xmlNodePtr libxml_node;

    std::vector<std::shared_ptr<XmlElement>> children;
    std::vector<std::string> text_nodes;

public:
    XmlElement(XmlDocument& doc, XmlElement* parent, xmlNodePtr libxml_node);
    ~XmlElement();

    unsigned short getXmlPosition() const;

    XmlDocument& getDocument();
    XmlElement* getParent();
    const std::vector<std::shared_ptr<XmlElement>>& getChildElements() const;

    const char* getName() const;
    void setName(const std::string& name);

    const char* getAttribute(const std::string& key) const;
    bool hasAttribute(const std::string& key) const;
    void setAttribute(const std::string& key, const std::string& value);

    const std::vector<std::string>& getTextNodes() const;
};

#endif // __XML_HH__
