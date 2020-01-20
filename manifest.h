//
// Created by syl on 1/19/20.
//

#ifndef SDK_CMAKE_BUILD_DEBUG_MANIFEST_H_
#define SDK_CMAKE_BUILD_DEBUG_MANIFEST_H_

#include <string>
#include <vector>
#include <unordered_map>

// The attempt of this class is to parse manifest file
// and replace all bits that annoy us like
// ../ in relative paths or non-relative paths
// When an url can be served the right way we will
// replaced in the manifest by a relative address with an uuid
// and we will keep an proxy uuid -> real addr
class manifest {
 private:
  std::vector<std::string> _content;

 public:
  //first string is uuid, bool is_relative_addr, second string is addr (relative or non)
  static std::unordered_map<std::string, std::pair<bool, std::string>> _proxy;

  manifest(std::string const &data);
  std::string get_manifest_processed_data();
};

#endif //SDK_CMAKE_BUILD_DEBUG_MANIFEST_H_
