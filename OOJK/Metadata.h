/**
 @file	Metadata.h.

 @brief	Declares the metadata class.
		Used as a cache container for all evaluated metadata, so subsequent reads to same files can be resolved faster.
		A program should use a singleton of this class for performance and memory consumption.
 */

#pragma once
#include <vector>
#include <map>
#include <memory>

typedef std::map<std::string, std::string> MetaContainer;	/** Defines structure for the metadata as a map of key-value pairs */

class Metadata {
private:
	static std::map<std::string, std::shared_ptr<MetaContainer>> cache;	/** Cached metadata is saved in a key-value map using the path as the key */
public:
	static std::shared_ptr<MetaContainer> getFileMetadata(const std::string &path);		/** Retrieves metadata corresponding a path */
	static MetaContainer readFileMetadata(const std::string &path);		/** Reads metadata from a specified file */
	static unsigned int getCount() noexcept;							/** Returns number of songs have metadata resolved */
	static void clear() noexcept;										/** Clears metadata */
};