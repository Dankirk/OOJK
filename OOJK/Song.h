/**
 @file	Song.h.

 @brief	Declares the song interface that songs in Playlist should adhere to.
 */

#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "Metadata.h"

class Song {

	public:
	virtual ~Song() = default;									/** Use default destructor */
	Song() = default;											/** Use default default constructor */
	Song(const Song&) = default;								/** Use default copy constructor */
	Song(Song&&) = default;										/** Use default move constructor */
	virtual Song& operator=(const Song&) = default;				/** Use defaut copy assignment operator */
	virtual Song& operator=(Song&&) = default;					/** Use default move assignment operator */
	friend std::ostream& operator<<(std::ostream&, const Song&);/** Song should be ostream printable */
	virtual std::ostream& print(std::ostream&) const = 0;		/** Actual implementation of "<<" */
	virtual std::shared_ptr<MetaContainer> evaluate() const = 0;/** Song should be evaluatable for metadata */
	virtual std::string getPath() const = 0;					/** Returns path to physical file */
	virtual std::unique_ptr<Song> clone() const = 0;			/** Song should be clonable */
	virtual bool operator==(const Song&) const;					/** Song should be comparable to other songs */
};
