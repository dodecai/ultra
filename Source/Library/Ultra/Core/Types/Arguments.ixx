export module Ultra.Core.Arguments;

import <string>;
import <vector>;

export namespace Ultra {

///
/// @brief Container for passed command line arguments
/// 
/// @example
/// Arguments arguments { argv + 1, argv + argc };
///
class Arguments {
private:
    using ArgumentList = std::vector<std::string>;

public:
    Arguments() = default;
    Arguments(const ArgumentList &arguments): mArgumentList(arguments) {}

private:
    ArgumentList mArgumentList = {};
};

}
