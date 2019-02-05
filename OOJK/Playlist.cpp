/**
 @file	Playlist.cpp.

 @brief	Implements the playlist class
 */

#include "Playlist.h"
#include <sstream>
#include <fstream>

 /**
  @fn	std::istream& operator>>(std::istream& is, Playlist& pl)

  @brief	Adds songs in input stream to playlist

  @param	is	Reference to input stream to parse.
			pl	Reference to playlist to add parsed songs to.

  @return	Reference to the input stream
  */

std::istream& operator>>(std::istream& is, Playlist& pl) {

	pl.load(is);
	return is;
}

/**
 @fn	std::ostream& operator<<(std::ostream& os, const Playlist& pl)

 @brief	Writes playlist songs to output stream

 @param		os	Reference to output stream to write playlist to.
			pl	Reference to playlist to write to output stream.

 @return	Reference to the output stream
 */

std::ostream& operator<<(std::ostream &os, const Playlist& pl)
{
	pl.print(os);
	return os;
}

 /**
  @fn	Playlist::~Playlist()

  @brief	Default destrcutor
  */

Playlist::~Playlist() {

}

/**
 @fn	Playlist::Playlist()

 @brief	Default constructor
 */

Playlist::Playlist() noexcept {

}

/**
 @fn	Playlist::Playlist(std::string savefile)

 @brief	Construction using a file containing filepaths to song files

 @param	savefile Path to file containing filepaths to song files.
 */

Playlist::Playlist(const std::string &savefile) {

	std::ifstream file(savefile);

	// Throw on failure to follow RAII for the Playlist object
	if (file.fail())
		throw std::runtime_error("Cannot open playlist file for reading");

	file >> *this;
	file.close();

	if (file.fail() && !file.eof())
		throw std::runtime_error("Error while reading playlist file");
}

/**
 @fn	Playlist::Playlist(std::istream& stream)

 @brief	Construction using a input stream containing filepaths to song files


 @param	stream Input stream containing filepaths to song files.
 */

Playlist::Playlist(std::istream &stream) {
	load(stream);
}

/**
 @fn	Playlist::Playlist(const Playlist& pl)

 @brief	Copy construction using reference to another playlist instance

 @param	pl	A reference to playlist to copy from
 */

Playlist::Playlist(const Playlist& pl) {
	*this = pl;
}

/**
 @fn	Playlist::Playlist(Playlist&& pl)

 @brief	Move construction using another playlist

 @param [in,out]	pl	Playlist to move from
 */

Playlist::Playlist(Playlist&& pl) noexcept : songs(std::move(pl.songs)) {
	pl.songs.clear();
}

/**
 @fn	Playlist::operator=(const Playlist& pl)

 @brief	Duplicates playlist contents to another playlist

 @param [in]	pl	Playlist to copy from

 @return Playlist& Reference to this playlist instance
 */

Playlist& Playlist::operator=(const Playlist& pl) {

	// Imitate copy constrcutor behaviour and swap results
	if (this != &pl) {
	
		// We know the number of elements to be added, so better reserve space for them.
		// This way no realloactions are needed between additions.
		songs.reserve(pl.songs.size());

		// Deep copy songs. We need const& to traverse the unique_ptr vector.
		/*
		std::transform(pl.songs.begin(), pl.songs.end(), std::back_inserter(songs), [](const SongElement& s) {
			return s->clone();
		});
		*/
		
		// This is C++17 equivalent to above transform()
		for (auto const& s : pl.songs) {
			songs.emplace_back(s->clone());
		}
	}

	return *this;
}

/**
 @fn	Playlist::Playlist(Playlist&& pl)

 @brief	Move assignment using another playlist

 @param [in,out]	pl	Playlist to move from
 
 @return Reference to this playlist instance
 */
Playlist& Playlist::operator=(Playlist&& pl) noexcept {

	if (this == &pl) 
		return *this;

	songs = std::move(pl.songs);
	pl.songs.clear();

	return *this;
}

/**
 @fn	void Playlist::evaluate()

 @brief	Converts all Songs to ConcreteSongs
 */

void Playlist::evaluate() {
	
	SongList newlist;

	// Reserve equal amount of space for newlist
	newlist.reserve(songs.size());

	for (auto const& song : songs) {
		newlist.emplace_back(std::make_unique<ConcreteSong>(
			song->getPath(),
			song->evaluate()
		));
	}

	// replace member songlist with the new one
	songs = std::move(newlist);
	// std::swap(newlist, songs);
}

/**
 @fn	std::list<std::reference_wrapper<const SongElement>> Playlist::evaluate(const Song& song)

 @brief	Finds the specified song(s) in the playlist and evaluates those

 @param	song	Song to find from playlist and evaluate. 
				A playlist may contain multiple instances the song.

@return		List of references to songs that were evaluated
 */

std::list<std::reference_wrapper<const SongElement>> Playlist::evaluate(const Song& song) {

	std::list<std::reference_wrapper<const SongElement>> evaluated;

	for (auto it = songs.begin(); it != songs.end(); it++) {
		if ((**it) == song) {
			*it = std::make_unique<ConcreteSong>(
				(**it).getPath(), 
				(**it).evaluate()
			);
			evaluated.push_back(std::cref(*it));
		}
	}
	return evaluated;
}


/**
 @fn	void Playlist::print(std::ostream& os) const

 @brief	Inserts all songs to given ostream

 @param [in,out]	os	The ostream to insert song representations in to.
 */

void Playlist::print(std::ostream& os) const {

	for (auto const& song : songs) {
		os << *song << std::endl;
	}
}

/**
 @fn	std::unique_ptr<Playlist> Playlist::clone() const

 @brief		Clones playlist to a new unique pointer

 @return	The unique pointer to a copy of this object.
 */

std::unique_ptr<Playlist> Playlist::clone() const {
	return std::make_unique<Playlist>(*this);
}

/**
 @fn	void Playlist::add(const Song& song)

 @brief			Adds a song to the playlist

 @param	song	The song to add.
 */

void Playlist::add(const Song& song) {
	songs.emplace_back(song.clone());
}

/**
 @fn			void Playlist::remove(const Song& song)

 @brief			Removes the given song from playlist

 @param	song	The song to remove.
 */

void Playlist::remove(const Song& song) {
	
	for (auto it = songs.begin(); it != songs.end();) {
		if (**it == song)
			it = songs.erase(it);
		else
			it++;
	}
}

/**
 @fn			unsigned int Playlist::getCount()

 @brief			Returns number of songs in the playlist

 @return unsigned int	Number of songs in the playlist
 */

unsigned int Playlist::getCount() noexcept {
	return songs.size();
}

/**
 @fn			void Playlist::clear()

 @brief			Removes all songs from the playlist

 */

void Playlist::clear() noexcept {
	songs.clear();
}

/**
 @fn			void Playlist::load(std::istream &is)

 @brief			Loads songs from inpust stream to playlist

 @param is		Input stream to read songs from
 */

void Playlist::load(std::istream &is) {

	std::string line;
	const std::string delimeter = ": ";
	const size_t delimeter_len = delimeter.length();

	while (std::getline(is, line)) {

		const size_t pos = line.find(delimeter);

		// Confirm line can be parsed correctly, if not, skip this line
		if (pos == std::string::npos || line.length() <= (pos + delimeter_len))
			continue;

		// Find where the path ends. (There should be another ": ")
		size_t endpos = line.find(delimeter, pos + delimeter_len);
		endpos = (endpos == std::string::npos) ? line.length() : endpos;
		endpos -= pos + delimeter_len;
		
		const std::string type = line.substr(0, pos);
		const std::string path = line.substr(pos + delimeter_len, endpos);
	
		if (type == "ProxySong") {
			add(ProxySong(path));
		}
		
		else if (type == "ConcreteSong") {
			add(ConcreteSong(path, Metadata::getFileMetadata(path)));
		}
		/*
		else {
			std::cout << "unsolved type: " << type << std::endl;
			std::cout << "unsolved path: " << path << std::endl;
		}
		*/
	}
}

/**
 @fn			void Playlist::writeToFile(const std::string& path)

 @brief			Writes playlist's songs into a file

 @param	path	Path to the file to write to
 */

void Playlist::writeToFile(const std::string& path) const {
	std::ofstream file(path, std::ios_base::trunc);

	if (file.fail())
		throw std::runtime_error("Cannot open playlist file for writing");

	file << *this;
	file.close();

	if (file.fail() && !file.eof())
		throw std::runtime_error("Error writing to playlist file");
}