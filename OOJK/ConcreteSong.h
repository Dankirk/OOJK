/**
 @file	ConcreteSong.h.

 @brief	Declares the concrete song class
		Used to represent song instaces in their largest form. Contains evaluated key-value based metadata for the song.
 */

#pragma once
#include "Song.h"

class ConcreteSong : public Song {
private:
	const static std::string title_strings[];				/** Contains metadata key strings that are used to display songs */
	std::string path;										/** Path to a physical file on storage media that can be evaluated for metadata */
	std::shared_ptr<MetaContainer> metadata;				/** Contains key-value based metadata */
public:
	~ConcreteSong();										/** Destrcutor */
	ConcreteSong() = delete;								/** Delete defalt constructor */
	explicit ConcreteSong(const std::string&, const std::shared_ptr<MetaContainer>&); /** Construction using reference to existing metadata */
	ConcreteSong(const ConcreteSong&);						/** Copy construction using lvalue reference to another instance */
	ConcreteSong(ConcreteSong&&) noexcept;					/** Move constructor using rvalue reference another instance */
	ConcreteSong& operator=(const ConcreteSong&);			/** Copy assignment using lvalue reference to another instance */
	ConcreteSong& operator=(ConcreteSong&&) noexcept;		/** Move assignment using rvalue reference another instance */

	std::ostream& print(std::ostream&) const override;		/** Print operator prints the metadata values using title_strings as keys */
	std::shared_ptr<MetaContainer> evaluate() const noexcept override;	/** Returns key-value based metadata for the song */
	std::string getPath() const override;					/** Returns path to physical file */
	std::unique_ptr<Song> clone() const override;			/** Clones the song into new unique pointer */
	bool operator==(const Song&) const override;			/** Can be compared to other songs using the same abstraction/interface */
	bool operator==(const ConcreteSong&) const;				/** Can be compared to other concrete songs (using contained metadata) */
	bool metadataEquals(const MetaContainer&) const;		/** Compares metadata with another set of metadata */

};
