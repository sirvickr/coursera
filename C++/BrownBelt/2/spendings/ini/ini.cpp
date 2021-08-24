#include "ini.h"

namespace Ini {

Section& Document::AddSection(string name) {
    return sections[name];
}

const Section& Document::GetSection(const string& name) const {
    return sections.at(name);
}

size_t Document::SectionCount() const {
    return 0;
}

}
