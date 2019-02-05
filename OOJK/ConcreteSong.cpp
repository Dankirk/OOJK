/**
 @file	ConcreteSong.cpp.

 @brief	Implements the concrete song class
 */

#include "ConcreteSong.h"

 /** A static ordered list of metadata keys used as to represent a concrete song */
const std::string ConcreteSong::title_strings[] = { "artist", "album", "title" };	

/**
 @fn	ConcreteSong::~ConcreteSong()

 @brief	Destructor
 */

ConcreteSong::~ConcreteSong() {

}

/**
 @fn	ConcreteSong::ConcreteSong(const MetaContainer& md)

 @brief	Construction using a reference to existing metadata

 @param	path Path to physical file
		md	Metadata to use as the member variable
 */

ConcreteSong::ConcreteSong(const std::string& p, const std::shared_ptr<MetaContainer>& md) : 
	path(p),
	metadata(md) 
{

}

/**
 @fn	ConcreteSong::ConcreteSong(const ConcreteSong& cs)

 @brief	Copy constructor

 @param	cs	Reference to concrete song to copy
 */

ConcreteSong::ConcreteSong(const ConcreteSong& cs) : 
	path(cs.path),
	metadata(cs.metadata)
{

}

/**
 @fn	ConcreteSong::ConcreteSong(ConcreteSong&& cs)

 @brief	Move constructor

 @param [in,out]	cs	Concrete song moved from
 */

ConcreteSong::ConcreteSong(ConcreteSong&& cs) noexcept : 
	path(std::move(cs.path)),
	metadata(std::move(cs.metadata))
{
	cs.path.clear();
	cs.metadata.reset();
}

ConcreteSong& ConcreteSong::operator=(const ConcreteSong& cs) {
	path = cs.path;
	metadata = cs.metadata;
	return *this;
}

ConcreteSong& ConcreteSong::operator=(ConcreteSong&& cs) noexcept {

	if (this == &cs)
		return *this;

	path = std::move(cs.path);
	metadata = std::move(cs.metadata);
	
	cs.path.clear();
	cs.metadata.reset();

	return *this;
}

/**
 @fn	std::unique_ptr<Song> ConcreteSong::clone() const

 @brief	Makes a deep copy of this object

 @return	A copy of this object as a new unique pointer
 */

std::unique_ptr<Song> ConcreteSong::clone() const {
	return std::make_unique<ConcreteSong>(*this);
}

/**
 @fn	MetaContainer ConcreteSong::evaluate() const

 @brief	Returns the metadata for the concrete song.

 @return	A MetaContainer containing metadata.
 */

std::shared_ptr<MetaContainer> ConcreteSong::evaluate() const noexcept {
	return metadata;
}

/**
 @fn	std::string ConcreteSong::getPath() const

 @brief	Returns path to the physical file for the concrete song.

 @return	Path to physical file
 */

std::string ConcreteSong::getPath() const {
	return path;
}

/**
 @fn	std::ostream& ConcreteSong::operator<<(std::ostream& os) const

 @brief	Stream insertion operator.
		A representation of the song is inserted to the given stream. 
		Metadata values at keys contained in title_strings are used to make this representation.

 @param [in,out]	os	The ostream to insert representation of the song to.

 @return	The modified output stream
 */

std::ostream& ConcreteSong::print(std::ostream& os) const {

	os << "ConcreteSong: " << path.c_str() << ":";

	for (const std::string& var : title_strings) {
		auto it = metadata->find(var);
		if (it != metadata->end())
			os << " " << (*it).second.c_str();
	}

	return os;
}

/**
 @fn	bool ConcreteSong::operator==(const Song& s) const

 @brief	Equality operator for other songs using same interface

 @param	s	Reference to other song to compare to.

 @return	True if the other song is also of ConcreteSong class and it's metadata 
			equals this instances metadata, otherwise false
 */

bool ConcreteSong::operator==(const Song& s) const {

	if (const ConcreteSong* cs = dynamic_cast<const ConcreteSong*>(&s))
		return (*this == *cs);

	return Song::operator==(s);
}

/**
 @fn	bool ConcreteSong::operator==(const ConcreteSong& cs) const

 @brief	Equality operator for other concrete songs

 @param	cs	Reference to a concrete song to compare to.

 @return	True if concrete songs have same path or contain same equal metadata, otherwise false
 */

bool ConcreteSong::operator==(const ConcreteSong& cs) const {
	return path == cs.getPath() || metadataEquals(*cs.metadata);
}

/**
 @fn	bool ConcreteSong::metadataEquals(const MetaContainer& other) const

 @brief	Compares sets of metadata to each other

 @param	other	Reference to metadata container to compare to

 @return	True if contained metadata is equal, otherwise false
 */

bool ConcreteSong::metadataEquals(const MetaContainer& other) const {
	return metadata->size() == other.size() && std::equal(metadata->begin(), metadata->end(), other.begin());
}