/**
 @file	ProxySong.cpp.

 @brief	Implements the proxy song class
 */

#include "ProxySong.h"

/**
 @fn	ProxySong::~ProxySong()

 @brief	Destructor
 */

ProxySong::~ProxySong() {

}

/**
 @fn	ProxySong::ProxySong(const std::string &filepath)

 @brief	Construction using a filepath.
		Why not const& ? Because we need to make a copy anyway for the member variable)

 @param	filepath	The filepath to a physical song file on a storage media.
 */

ProxySong::ProxySong(const std::string &filepath) : path(filepath) {
}

/**
 @fn	ProxySong::ProxySong(const ProxySong& ps)

 @brief	Copy constructor using lvalue reference to ProxySong

 @param	ps	Reference to ProxySong instance to copy from
 */

ProxySong::ProxySong(const ProxySong& ps) : path(ps.path) {

}

/**
 @fn	ProxySong::ProxySong(ProxySong&& ps)

 @brief	Move constructor using ProxySong rvalue reference

 @param [in,out]	ps	ProxySong to move from
 */

ProxySong::ProxySong(ProxySong&& ps) noexcept : path(std::move(ps.path)) {

	//ps.path.clear();
}

/**
 @fn	ProxySong::operator=(const ProxySong& ps)

 @brief	Copy assignment operator using lvalue reference

 @param [in]	ps	ProxySong reference to copy from
 */

ProxySong& ProxySong::operator=(const ProxySong& ps) {
	path = ps.path;
	return *this;
}

/**
 @fn	ProxySong::opearator=(ProxySong&& ps)

 @brief	Move assignment operator using rvalue refernce

 @param [in,out]	ps	ProxySong reference to move from
 */

ProxySong& ProxySong::operator=(ProxySong&& ps) noexcept {
	
	if (this == &ps)
		return *this;

	path = std::move(ps.path);
	//ps.path.clear();
	
	return *this;
}

/**
 @fn	std::unique_ptr<Song> ProxySong::clone() const

 @brief	Makes a deep copy of this object

 @return	A copy of this object as a new unique pointer.
 */

std::unique_ptr<Song> ProxySong::clone() const {
	return std::make_unique<ProxySong>(*this);
}

/**
 @fn	MetaContainer ProxySong::evaluate() const

 @brief	Finds metadata representing the file based on it's filepath.

 @return	A MetaContainer containing metadata.
 */

std::shared_ptr<MetaContainer> ProxySong::evaluate() const {
	return Metadata::getFileMetadata(path);
}

/**
 @fn	std::string ProxySong::getPath() const

 @brief	Returns path to the physical file for the concrete song.

 @return	Path to physical file
 */

std::string ProxySong::getPath() const {
	return path;
}

/**
 @fn	std::ostream& ProxySong::operator<<(std::ostream& os) const

 @brief	Stream insertion operator.
		A representation of the song is inserted to the given stream.
		Filepath is used as the representation.

 @param [in,out]	os	The ostream to insert representation of the song to.

 @return	The modified stream
 */

std::ostream& ProxySong::print(std::ostream& os) const {
	return os << "ProxySong: " << path.c_str();
}

/**
 @fn	bool ProxySong::operator==(const Song& s) const

 @brief	Equality operator for other songs using same interface

 @param	s	Reference to other song to compare to.

 @return	True if the other song is also of ProxySong class and it's path equals this instances path, otherwise false
 */

bool ProxySong::operator==(const Song& s) const {

	if (const ProxySong* ps = dynamic_cast<const ProxySong*>(&s))
		return (*this == *ps);
	
	return Song::operator==(s);
}

/**
 @fn	bool ProxySong::operator==(const ProxySong& ps) const noexcept

 @brief	Equality operator for other proxy songs

 @param	ps	Reference to a proxy song to compare to.

 @return	True if paths are equal, otherwise false
 */

bool ProxySong::operator==(const ProxySong& ps) const noexcept {
	return (path == ps.path);
}