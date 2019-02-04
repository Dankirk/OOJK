/**
 @file	OOJK.cpp.

 @brief	Contains tests cases for OOJK exercice project
 */

#define CATCH_CONFIG_WINDOWS_CRTDBG	// Memory leak checks (for Windows)
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
//#define CATCH_CONFIG_RUNNER	// Replacement for CATCH_CONFIG_MAIN to allow custom main()
#include "catch.hpp"

/*
// For custom memory leak checking in main()
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 
*/

#include "Playlist.h"
#include "ProxySong.h"
#include "ConcreteSong.h"

TEST_CASE("Print playlist", "[print_playlist]") {

	Playlist pl1;

	// Use brackets to define scope for ProxySongs
	{
		ProxySong song1("/dummy/path/to/file1.mp3");
		ProxySong song2("/dummy/path/to/file2.mp3");
		ProxySong song3("/dummy/path/to/file3.mp3");

		pl1.add(song1);
		pl1.add(song2);
		pl1.add(song3);
	}

	// Test additions to pl1
	REQUIRE(pl1.getCount() == 3);

	ProxySong song1("/dummy/path/to/file1.mp3");

	Playlist pl2 = pl1;
	pl2.add(song1);

	// pl2 should have pl1 songs(3) + the newly added song1 = 4 songs
	REQUIRE(pl2.getCount() == 4);

	// Stringstreams for print testing
	std::stringstream test_print("", std::ios_base::in | std::ios_base::out | std::ios_base::ate);
	std::stringstream expected_print;

	// Confirm pl1 contains and prints expected list of ProxySongs
	SECTION("Print pl1") {

		// test print()
		pl1.print(test_print);

		expected_print
			<< "ProxySong: /dummy/path/to/file1.mp3" << std::endl
			<< "ProxySong: /dummy/path/to/file2.mp3" << std::endl
			<< "ProxySong: /dummy/path/to/file3.mp3" << std::endl;

		REQUIRE(test_print.str() == expected_print.str());

		// Test << operator
		test_print.str("");
		test_print << pl1;

		REQUIRE(test_print.str() == expected_print.str());
	}

	// Confirm pl2 contains and prints excepted list of ProxySongs
	SECTION("Print pl2") {

		pl2.print(test_print);

		expected_print
			<< "ProxySong: /dummy/path/to/file1.mp3" << std::endl
			<< "ProxySong: /dummy/path/to/file2.mp3" << std::endl
			<< "ProxySong: /dummy/path/to/file3.mp3" << std::endl
			<< "ProxySong: /dummy/path/to/file1.mp3" << std::endl;

		REQUIRE(test_print.str() == expected_print.str());
	}

	// Test song removal from playlists
	SECTION("Song removal from playlist") {

		// Removing songs from playlist only affects the copy
		pl1.remove(song1);
		REQUIRE(pl1.has(song1) == false);
		REQUIRE(pl2.has(song1) == true);
		REQUIRE(pl1.getCount() == 2);
		REQUIRE(pl2.getCount() == 4);

		// Should remove two instances of song1 on pl2, hence it's size should go from 4 -> 2
		pl2.remove(song1);
		REQUIRE(pl2.has(song1) == false);
		REQUIRE(pl1.getCount() == 2);
		REQUIRE(pl2.getCount() == 2);
	}

	// Confirm static Metadata cache grows when a specific Song is evaluated
	SECTION("Evaluate specific songs in playlist") {
		Metadata::clear();

		REQUIRE(Metadata::getCount() == 0);
		pl1.evaluate(song1);
		REQUIRE(Metadata::getCount() == 1);
	}

	// Confirm playlist evaluations replace elements correctly
	SECTION("Evaluate playlist") {

		Metadata::clear();

		// Monitor metadata cache growth for each playlist evaluates
		REQUIRE(Metadata::getCount() == 0);
		pl1.evaluate();
		REQUIRE(Metadata::getCount() == 3);
		pl2.evaluate();
		REQUIRE(Metadata::getCount() == 3);

		// Clear metadata, so metadata shared_ptr should only exist in pl1 and pl2 songlists
		// and still be printable in test_print
		Metadata::clear();
		REQUIRE(Metadata::getCount() == 0);

		// Check outputs
		expected_print
			<< "ConcreteSong: /dummy/path/to/file1.mp3: Some One The Album file1.mp3" << std::endl
			<< "ConcreteSong: /dummy/path/to/file2.mp3: Some One The Album file2.mp3" << std::endl
			<< "ConcreteSong: /dummy/path/to/file3.mp3: Some One The Album file3.mp3" << std::endl
			<< "ConcreteSong: /dummy/path/to/file1.mp3: Some One The Album file1.mp3" << std::endl;

		// Test print()
		pl2.print(test_print);
		REQUIRE(test_print.str() == expected_print.str());

		// Test playlist's << operator (should be equal to print())
		test_print.str("");
		test_print << pl2;
		REQUIRE(test_print.str() == expected_print.str());

		// Re-evaluation should not affect contents
		pl2.evaluate();

		test_print.str("");
		pl2.print(test_print);
		REQUIRE(test_print.str() == expected_print.str());
	}

	SECTION("Test copy and move constructors") {
		
		ProxySong orig("/dummy/path/to/file1.mp3");
		ProxySong copied(orig);
		REQUIRE(orig == copied);

		ProxySong moved(std::move(orig));
		REQUIRE(copied == moved);

		ConcreteSong cs("/dummy/path/to/file1.mp3", moved.evaluate());
		ConcreteSong copied_cs(cs);
		REQUIRE(cs == copied_cs);
		
		ConcreteSong moved_cs(std::move(cs));
		REQUIRE(copied_cs == moved_cs);
		REQUIRE(copied_cs == copied);
	}

	SECTION("Test copy and move assignments") {

		ProxySong orig("/dummy/path/to/file1.mp3");
		ProxySong copied("/not/to/be");
		copied = orig;
		REQUIRE(orig == copied);
		
		ProxySong moved("/not/to/be");
		moved = std::move(orig);
		REQUIRE(copied == moved);

		// Self assignment test
		moved = std::move(moved);
		REQUIRE(copied == moved);

		ConcreteSong cs("/dummy/path/to/file1.mp3", moved.evaluate());
		ConcreteSong copied_cs("/not/to/be", moved.evaluate());
		copied_cs = cs;
		REQUIRE(cs == copied_cs);
		
		ConcreteSong moved_cs("/not/to/be", moved.evaluate());
		moved_cs = std::move(cs);
		REQUIRE(copied_cs == moved_cs);
		REQUIRE(copied_cs == copied);

		// Self assignment test
		moved_cs = std::move(moved_cs);
		REQUIRE(copied_cs == moved_cs);
	}

	SECTION("playlist clone") {
		std::unique_ptr<Playlist> cloned_pl = pl1.clone();
		
		REQUIRE(cloned_pl->getCount() == 3);
		REQUIRE(pl1.getCount() == 3);
	}

	SECTION("playlist move") {

		REQUIRE(pl1.getCount() == 3);

		// Move assignments
		Playlist pl4;
		pl4 = std::move(pl1);

		// Move constructor
		Playlist pl5(std::move(pl4));

		// Self assignment
		pl5 = std::move(pl5);

		REQUIRE(pl5.getCount() == 3);
		REQUIRE(pl1.getCount() == 0);
		REQUIRE(pl4.getCount() == 0);
	}

	SECTION("Song equality in playlist (template usage)") {

		pl1.clear();
		pl2.clear();

		ProxySong song4("/dummy/path/to/file4.mp3");
		pl2.add(song4);

		REQUIRE(pl1.has(song4) == false);
		REQUIRE(pl2.has(song4) == true);

		// song4 (ProxySong) should still be found in pl2, even though it's now a ConcreteSong
		pl2.evaluate();
		REQUIRE(pl2.has(song4) == true);
	}

	SECTION("Test playlist creation from input stream") {

		const std::string song4_path = "/dummy/path/to/file4.mp3";
		const std::string song5_path = "/dummy/path/to/file5.mp3";
		const std::string song5_other_path = "/same/song/different/path/file5.mp3";
		const std::string tmpfile_path = "tmp_playlist.txt";

		// Put song in stringstream which is used to create a playlist
		// This also contains invalid entries that should be skipped
		test_print	<< "ProxySong: " << song4_path << std::endl
					<< "ConcreteSong: " << song5_other_path << ": Some One The Album file5.mp3" << std::endl
					<< "Invalid line: This should not be added as a song: K?" << std::endl
					<< "Another invalid line without delimeters" << std::endl
					<< "Invalid line without linebreak";
		Playlist pl3(test_print);

		// Confirm song4 is correctly parsed as a ProxySong into playlist
		ProxySong song4(song4_path);
		ConcreteSong song5(song5_path, Metadata::getFileMetadata(song5_path));
		REQUIRE(pl3.has(song4) == true);
		REQUIRE(pl3.has(song5) == true);
		REQUIRE(pl3.getCount() == 2);

		// Write playlist to temp file
		pl3.writeToFile(tmpfile_path);

		// Load songs from tmp file
		Playlist pl4(tmpfile_path);

		// Remove temp file
		remove(tmpfile_path.c_str());

		// Confirm (only) song4 was parsed from the file
		REQUIRE(pl4.has(song4) == true);
		REQUIRE(pl4.has(song5) == true);
		REQUIRE(pl4.getCount() == 2);
	}

	SECTION("File error handling") {

		REQUIRE_THROWS_WITH(Playlist("/does/not/exist/:?|"), "Cannot open playlist file for reading");
		REQUIRE_THROWS_WITH(pl1.writeToFile("/inaccessible/:?|"), "Cannot open playlist file for writing");

	}

	// Clear statics here for memory leak checking
	Metadata::clear();	
}

/**
 @fn	int main(int argc, char* argv[])

 @brief	Custom entry point to test memory leaks

 @param [in]	argc	Number of commandline parameters
		[in]	argv	char array of commandline arguments

@return int		Error code
 */

/*
int main(int argc, char* argv[]) {

	_CrtMemState state;
	int result = 0;
	_CrtMemCheckpoint(&state); 
	{
		result = Catch::Session().run(argc, argv);
	}
	_CrtMemDumpAllObjectsSince(&state);
	return result;
}
*/

