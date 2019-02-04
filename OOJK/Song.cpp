#pragma once
#include "Song.h"

/**
 @fn	Song::operator<<(std::ostream& stream, const Song& song)

 @brief	Puts song into output stream in printable format

@param	stream	Reference to output stream to print song to
		song	Reference to song to put to output stream

@return std::ostream& Reference to output stream
 */

std::ostream& operator<<(std::ostream& stream, const Song& song) {
	return song.print(stream);
}

/**
 @fn	Song::operator==(const Song& rhs)

 @brief	Default comparison operator in case comparable instances
		cannot be casted to inherited types.

@param rhs	Reference to song to compare to

@return bool True if paths are equal, otherwise false
 */

bool Song::operator==(const Song& rhs) const {
	return getPath() == rhs.getPath();
}