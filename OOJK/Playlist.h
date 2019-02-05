/**
 @file	Playlist.h.

 @brief	Declares the playlist class
		Controls a list of songs implementing Song interface
		Can evaluate Song (containing only filepath) into ConcreteSong (containing metadata)
 */

#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <list>

#include "Song.h"
#include "ConcreteSong.h"
#include "ProxySong.h"

typedef std::unique_ptr<Song> SongElement;	/** Convenience typedef for songs */
typedef std::vector<SongElement> SongList;	/** Convenience typedef for container of songs */

class Playlist {

	friend std::ostream& operator<<(std::ostream&, const Playlist&);	/** Inserts all songs to given ostream */
	friend std::istream& operator>>(std::istream&, Playlist&);		/** Add songs from a file */

protected:
	SongList songs;									/** List of songs (that implement Song interface) in the playlist */

public:
	~Playlist();									/** Desctructor */
	Playlist() noexcept;							/** Default constructor */
	explicit Playlist(const std::string &savefile);	/** Construction using a file containing filepaths to song files */
	explicit Playlist(std::istream&);				/** Constrcution using a input stream */
	Playlist(const Playlist&);						/** Copy constructor using a lvalue reference to another playlist */
	Playlist(Playlist&&) noexcept;					/** Move constructor using a rvalue reference to another playlist */
	Playlist& operator=(const Playlist&);			/** Copy assignment using a lvalue reference to another playlist */
	Playlist& operator=(Playlist&&) noexcept;		/** Move assignment using a rvalue reference to another playlist */

	void evaluate();								/** Converts all Songs to ConcreteSongs */
	std::list<std::reference_wrapper<const SongElement>> evaluate(const Song&); /** Converts a given Song to ConcreteSong */
	std::unique_ptr<Playlist> clone() const;		/** Clones playlist to a new unique pointer */
	void print(std::ostream&) const;				/** Inserts all songs to given ostream */
	void load(std::istream&);						/** Loads songs from input stream */
	void writeToFile(const std::string&) const;		/** Writes playlist to file */
	
	void add(const Song& song);						/** Adds Song to songlist */
	void remove(const Song& song);					/** Removes Song from songlist */
	unsigned int getCount() noexcept;				/** Returns number of songs in the playlist */
	void clear() noexcept;							/** Removes all songs from the playlist */

	/**
	 @fn			void Playlist::has(const T& song)

	 @brief			Determines existance of specified song in playlist

	 @param song	Reference to song to find from playlist

	 @return		TRUE if song exists in playlist, otherwise false
	 */

	// Implemented in .h, because templates cannot be implemented in .cpp 
	// without losing the genericness
	template <class T>
	bool has(const T& song) {
		for (auto it = songs.begin(); it != songs.end(); it++) {
			if (**it == song)
				return true;
		}
		return false;
	}
};