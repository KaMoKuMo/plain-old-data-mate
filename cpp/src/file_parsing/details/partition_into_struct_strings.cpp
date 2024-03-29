
#include "file_parsing/details/partition_into_struct_strings.h"

#include "file_parsing/details/find_inner_scope.h"
#include "file_parsing/details/trim.h"

/**
 * Partitions a cpp file content into a vector of struct strings.
 * If the content does not contain a struct definition no error is emitted,
 * but an empty result range
 *
 * \param cppFileContent to be partitioned
 * \return collection of \see StructsRawData if processable,
 *         else a description of the problem
 **/
Expected<std::vector<StructsRawData>, std::string>
partitionIntoStructStrings(std::string const& cppFileContent) {
    std::vector<StructsRawData> result;
    size_t offset = 0;
    while (true){
        offset = cppFileContent.find("struct", offset);
        if (offset == std::string::npos) {
            break;
        }
        auto expectedScope = findInnerScope(cppFileContent, offset);
        if (!expectedScope)
            return Unexpected(std::move(expectedScope).error());

        //Only if a scope can be found same members are returned
        if (expectedScope->first != expectedScope->second) {
            result.emplace_back(
                StructsRawData{
                    trim( std::string( cppFileContent.begin() + static_cast<long>(offset) + 6, expectedScope->first - 1)), //name des structs
                    std::string(expectedScope->first, expectedScope->second),
                    offset
                }
            );
        }
        offset = static_cast<size_t>(expectedScope->second - cppFileContent.begin());
    }
    return result;
}
