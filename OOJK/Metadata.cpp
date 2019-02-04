/**
 @file	Metadata.cpp.

 @brief	Implements the metadata class
 */

#include "Metadata.h"

// Initialize static members
std::map<std::string, std::shared_ptr<MetaContainer>> Metadata::cache = {};

/**
 @fn	std::shared_ptr<MetaContainer> Metadata::getFileMetadata(const std::string &path)

 @brief	Gets file metadata corresponding given path.
		Tries to find and return the metadata from cache. If no entry for the path exists,
		calls readFileMetadata() to acquire metadata, save it in the cache and return it.

 @param	path	Full pathname of the file to read metadata from.

 @return	Shared pointer to file metadata.
 */

std::shared_ptr<MetaContainer> Metadata::getFileMetadata(const std::string &path) {

	// Try to find metadata from cache
	auto it = cache.find(path);

	if (it != cache.end())
		return it->second;

	// Load and cache metadata for later use
	cache[path] = std::make_shared<MetaContainer>(readFileMetadata(path));

	return cache[path];
}

/**
 @fn	MetaContainer Metadata::readFileMetadata(const std::string &path)

 @brief	Reads file metadata from file.
		This is supposed to be an expensive function, so results should be cached

 @param	path	Full pathname of the file.

 @return	The file metadata container.
 */

MetaContainer Metadata::readFileMetadata(const std::string &path) {

	// Figure out filename from path. 
	// If no folder delimeters (/ or \) are found, use the whole string.
	size_t last_delimeter = path.find_last_of("/\\");
	last_delimeter = (last_delimeter == std::string::npos) ? 0 : (last_delimeter + 1);
	std::string filename = path.substr(last_delimeter);
	
	// Use some dummmy data as metadata to represent mp3 IDV3 tags
	MetaContainer metadata;

	metadata["copyright"] = "Some One";
	metadata["artist"] = "Some One";
	metadata["album"] = "The Album";
	metadata["title"] = filename;

	return metadata;
}

/**
 @fn			unsigned int Metadata::getCount()

 @brief			Returns number of songs in the metadata cache

 */

unsigned int Metadata::getCount() noexcept {
	return cache.size();
}

/**
 @fn			void Metadata::clear()

 @brief			Clears metadata cache

 */

void Metadata::clear() noexcept {
	cache.clear();
}