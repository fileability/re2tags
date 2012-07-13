#import "definition.hpp"
#import "utils.hpp"
#import <stdio.h>

namespace rtt {

RE2* cachedRegexForString(std::string str) {
    static std::map<std::string, RE2*> cache;
    if (cache.count(str))
        return cache[str];
    
    RE2* compiledRegex = new RE2(str);
    cache[str] = compiledRegex;
    return cache[str];
}

static std::vector<std::string> JsonStringListToCPPStringList(Json::Value& j) {
    std::vector<std::string> strs;
    for (Json::Value& strj : j) {
        strs.push_back(strj.asString());
    }
    return strs;
}
Language::Language(Json::Value& j) : symbols(), name(), extensions() {
    extensions = JsonStringListToCPPStringList(j["exts"]);
    
    for (Json::Value& symbolj : j["symbols"]) {
        SymbolDef symdef = SymbolDef();
        symdef.kind = symbolj["kind"].asString();
        symdef.sourceRegex = symbolj["regex"].asString();
        if (symbolj.isMember("scope")) {
            Json::Value& scopej = symbolj["scope"];
            symdef.scoped = JsonStringListToCPPStringList(scopej);
        }
        symbols.push_back(symdef);
    }
    printf("$regex = %lu\n", symbols.front()._regex);
}

Manager Manager::getManager(std::string path_to_definitions) {
    static Manager m(path_to_definitions);
    return m;
}
Manager::Manager(std::string path_to_definitions) : langs() {
    std::ifstream stream(path_to_definitions.c_str(), std::ifstream::in);
    Json::Reader reader;
    
    Json::Value root;
    reader.parse(stream, root);
    
    for (std::string langname : root.getMemberNames()) {
        Language lang(root[langname]);
        lang.name = langname;
        langs.push_back(lang);
    }
}
Language* Manager::detectLanguage(std::string path, std::string content) {
    for (Language& lang : langs) {
        printf("symbols = %u\n", lang.symbols.size());
        for (std::string& ext : lang.extensions) {
            std::string dot_ext = std::string(".") + ext;
            printf("path = [%s]\n", path.c_str());
            printf("ext = [%s]\n", dot_ext.c_str());
            if (string_ends_with(path, dot_ext)) {
                return &lang;
            }
        }
    }
    return nullptr;
}

}