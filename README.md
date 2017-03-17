# B-plus-Tree-Indexing

Authors:
 /*
 * 		Deepak Shanmugam
      Chandra Sekhar Guntupalli
 *
 */
 
Description of the program:
  This program that implements B+ tree indexing reads a text file containing data and build the index, treating the first 15 columns as the key.

The structure of the index is as follows: First 256 bytes: Name of the text file you have indexed. This must be blank-filled on the right.  This may need other metadata in this first block, so it is suggested you allocate 1K so you can read it in as a block. The rest of the file is 1K blocks of index data, according to the way a B+ tree is structured. This implies that the program reads in a block of data, manipulate it, and possibly write it back out as a block. It uses a long (8-byte) record address for your pointers.  These "pointers" are the byte offset in the text file of the data record.  Note that the structure mixes text data(the key) and binary data(the pointer). This program buffers at most 3 disk blocks.

This program does the following functions:
1.Creating an index
2.Finding a record by key
3.Inserting a new text record
4.Listing sequential records 


How to execute:

		g++ -o INDEX BPlusIndexing.cpp

How to run:

1) To create Index:

		./INDEX -create <input file> <output file> <key size>

2) To find a record by key:

		./INDEX -find <index filename> <key>

3) To insert a new text record:

		./INDEX -insert <index filename> "new text line to be inserted."

4) To list sequential records:
	
		./INDEX -list <index filename> <starting key> <count>
