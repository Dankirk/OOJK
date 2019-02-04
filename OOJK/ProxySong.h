/**
 @file	ProxySong.h.

 @brief	Declares the proxy song class.
		Used to represent song files on storage media, that are not evaluated for metadata
 */

#pragma once
#include "Song.h"

class ProxySong : public Song {

private:
	std::string path;	/** path to a physical file on storage media that can be evaluated for metadata */

public:
	~ProxySong();												/** Destrcutor */
	ProxySong() = delete;										/** Delete default constructor */
	explicit ProxySong(const std::string &filepath);			/** Construction using a filepath */
	ProxySong(const ProxySong&);								/** Copy construction using a lvalue reference to another instance */
	ProxySong(ProxySong&&) noexcept;							/** Move construction using a rvalue reference another instance */
	ProxySong& operator=(const ProxySong&);						/** Copy assignment using a lvalue reference to another instance */
	ProxySong& operator=(ProxySong&&) noexcept;					/** Move assignment using a rvalue reference to another instance */

	std::ostream& print(std::ostream&) const override;			/** Print operator prints the path */
	std::shared_ptr<MetaContainer> evaluate() const override;	/** Finds file using a reference to metadata cache */
	std::string getPath() const override;						/** Returns path to physical file */
	std::unique_ptr<Song> clone() const override;				/** Clones the song into new unique pointer */
	bool operator==(const Song&) const override;				/** Can be compared to other songs using the same abstraction/interface */
	bool operator==(const ProxySong&) const noexcept;			/** Can be compared to other proxy songs (using path comparison) */
};
