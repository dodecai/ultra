export module Ultra.Core.Arguments;

import <string>;
import <vector>;

using std::string;
using std::vector;

export namespace Ultra {

///
/// @brief List of passed command line arguments
///
struct Arguments {
    Arguments() = default;
    Arguments(const vector<string> &arguments): mArgumentList(arguments) {}

private:
    vector<string> mArgumentList = {};
};

}
