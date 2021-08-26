#include "ini.h"
namespace Ini {

pair<string_view, string_view> Split(string_view line, char by) {
  size_t pos = line.find(by);
  string_view left = line.substr(0, pos);

  if (pos < line.size() && pos + 1 < line.size()) {
    return {left, line.substr(pos + 1)};
  } else {
    return {left, string_view()};
  }
}

Section& Document::AddSection(string name) {
    return sections[name];
}

const Section& Document::GetSection(const string& name) const {
    return sections.at(name);
}

size_t Document::SectionCount() const {
    return sections.size();
}

Document Load(istream& input) {
    Document doc;
    string line;
    Section* section = nullptr;
    while(getline(input, line)) {
        if(!line.empty()) {
            if(line[0] == '[') {
                section = &doc.AddSection(line.substr(1, line.size() - 2));
            } else {
                auto [name, value] = Split(line, '=');
                (*section)[string(name)] = string(value);
            }
        }
    }
    return doc;
}

}
