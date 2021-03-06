#include "JsonData.h"

void JsonData::prettify() {
    std::vector<char> formatted;
    int               tabs                 = 0;
    bool              in_string            = false;
    bool              newline_after_char   = false;
    bool              add_space_after_char = false;

    auto newline = [&]() {
        formatted.push_back('\n');
        for(int i = 0; i < tabs; ++i)
            formatted.push_back('\t');
    };

    for(auto it = m_data.begin(); it != m_data.end(); ++it) {
        if(!in_string) {
            if(*it == ',') {
                newline_after_char = true;
            } else if(*it == ':') {
                add_space_after_char = true;
            } else if(*it == '{') {
                // check for empty object - if the next char closes it
                if(*(it + 1) != '}') {
                    newline_after_char = true;
                    tabs++;
                } else {
                    formatted.push_back(*it++);
                }
            } else if(*it == '}') {
                tabs--;
                newline();
            } else if(*it == '[') {
                // check for empty array - if the next char closes it
                if(*(it + 1) != ']') {
                    newline_after_char = true;
                    tabs++;
                } else {
                    formatted.push_back(*it++);
                }
            } else if(*it == ']') {
                tabs--;
                newline();
            } else if(*it == '"') {
                in_string = true;
            }
        } else if(*it == '"') {
            auto rit             = it - 1;
            int  num_backslashes = 0;

            while(*rit-- == '\\')
                num_backslashes++;

            if(num_backslashes % 2 == 0)
                in_string = false;
        }

        formatted.push_back(*it);

        if(add_space_after_char) {
            formatted.push_back(' ');
            add_space_after_char = false;
        }

        if(newline_after_char) {
            newline();
            newline_after_char = false;
        }
    }

    m_data = formatted;
}
