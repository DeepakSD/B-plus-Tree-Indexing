#include <iostream>
#include "btTable.h"

/**
 * 
 */

/**
 * @author Deepak Shanmugam
           Chandra Sekhar Guntupalli
 *
 */

/**
* Function used to readline from the InputFile
* InputFile - file stream already opened for input on input text file
* Return - Word - char array, the word read in
*           Definition - char array, the definition read in
*/
long ReadLine(fstream & InputFile, KeyFieldType Word,
   DataFieldType Definition)
{
    char Line[LineMax];
    int k, ch;
    InputFile.getline(Line, LineMax);

    for (k = 0; k < KeyFieldMax; k++)
        Word[k] = Line[k];
    Word[KeyFieldMax] = NULLCHAR;

    int currentLength = strlen(Line);
    Definition = offset;
    offset += currentLength;
    return Definition;
}

/**
* Function to read the data from InputFile and load it into the Table
* InputFile - file stream already opened for input
* Return - Table containing the data
*/
void createIndex(fstream & InputFile, BTTableClass & Table)
{
    ItemType Item;
    int Count;
    Count = 0;
    Item.DataField = ReadLine(InputFile, Item.KeyField, Item.DataField);
    // DEBUGGING
    //cout << " createIndexKey: " << Item.KeyField << endl;
    //cout << " createIndexOffset: " << Item.DataField << endl;
    while (! InputFile.fail())
    {
        Table.Insert(Item);
        Item.DataField = ReadLine(InputFile, Item.KeyField, Item.DataField);
    }
}


int main(int argc, char* argv[])
{

    // Creating New File
    if (strcmp(argv[1],"-create") == 0)
    {
        if(argc!=5){
            cout<<"Improper Command"<<endl;
            cout<<"Try: ./INDEX -create <input file> <output file> <key size>"<<endl;
            return 0;
        }
            
        KeyFieldMax = atoi(argv[4]);
        InputFileName = argv[2];
        IndexFileName = argv[3];

        fstream Source;
        BTTableClass BTTable('w', IndexFileName);

        Source.open(InputFileName, ios::in);
        if (Source.fail())
        {
            cerr << "ERROR: Unable to open file "<< InputFileName << endl;
            exit(1);
        }

        createIndex(Source, BTTable);
        Source.close();
        return 0;
    }

    // Searching record by key
    if (strcmp(argv[1],"-find") == 0)
    {
        if(argc!=4){
            cout<<"Improper Command"<<endl;
            cout<<"Try: ./INDEX -find <index filename> <key>"<<endl;
            return 0;
        }
        ItemType Item;
        char TempSearchKey[30];
        char SearchKey [30];
        IndexFileName = argv[2];
        strcpy(TempSearchKey,argv[3]);
        BTTableClass BTTable('r', IndexFileName);
        if (BTTable.Empty())
            Error("Table is empty");
        int i;
        for (i=0;i<KeyFieldMax;i++)
        {
            SearchKey[i]=TempSearchKey[i];
        }
        SearchKey[i]= NULLCHAR;

        if (BTTable.Retrieve(SearchKey, Item))
        {
            fstream OldInputFile;
            string line;
            OldInputFile.open(InputFileName, ios::in | ios::binary);
            while (!OldInputFile.eof())
            {
                if (offset == Item.DataField)
                {
                    getline(OldInputFile, line);
                    cout << "At " << offset << ", record: " << line << endl;
                    break;
                }
                getline(OldInputFile, line);
                int currentLength = line.length();
                offset += currentLength;
            }
        }
        else
            cout << "Not found" << endl;
    return 0;
    }

    // Inserting new Record into Index file
    if (strcmp(argv[1],"-insert") == 0)
    {
        if(argc!=4){
            cout<<"Improper Command"<<endl;
            cout<<"Try: ./INDEX -insert <index filename> new text line to be inserted."<<endl;
            return 0;
        }
        ItemType Item;
        IndexFileName = argv[2];
        char Record [50];
        char SearchKey [15];
        bool notfound = false;
        strcpy(Record,argv[3]);
        // ** Really Good Stuff **
        // Parenthesis used to call destructor automatically
        {
            BTTableClass BTTable('r', IndexFileName);
            if (BTTable.Empty())
                Error("Table is empty");
            int i;
            for (i=0;i<KeyFieldMax;i++)
            {
                SearchKey[i]=Record[i];
            }
            SearchKey[i]= NULLCHAR;
            if (BTTable.Retrieve(SearchKey, Item))
            {
                cout << "Record Found; Not Inserted"<<endl;;
            }
            else
                notfound = true;
        }
        if (notfound)
        {
            // Recreate new file
            // TODO: Dynamic update
            fstream OldInputFile;
            string line;
            OldInputFile.open(InputFileName, ios::out | ios::app);
            OldInputFile <<'\n';
            OldInputFile << argv[3];
            OldInputFile.close();
            fstream Source;
            BTTableClass BTTable('w', IndexFileName);

            Source.open(InputFileName, ios::in);
            if (Source.fail())
            {
                cerr << "ERROR: Unable to open file "<<InputFileName << endl;
                exit(1);
            }
            createIndex(Source, BTTable);
            Source.close();
        }
        return 0;
    }

    if (strcmp(argv[1],"-list") == 0)
    {
        if(argc!=5){
            cout<<"Improper Command"<<endl;
            cout<<"Try: ./INDEX -list <index filename> <starting key> <count>"<<endl;
            return 0;
        }
        // Retrieves list of records in that particular branch
        ItemType Item;
        IndexFileName = argv[2];
        char TempSearchKey [30];
        char SearchKey [30];
        strcpy(TempSearchKey,argv[3]);
        ListCount = atoi(argv[4]);

        BTTableClass BTTable('r', IndexFileName);
        if (BTTable.Empty())
            Error("Table is empty");
        int i;
        for (i=0;i<KeyFieldMax;i++)
        {
            SearchKey[i]=TempSearchKey[i];
        }
        SearchKey[i]= NULLCHAR;
        //BTTable.RetrieveList(SearchKey, Item);
        if (BTTable.Retrieve(SearchKey, Item))
        {
            fstream OldInputFile;
            string line;
            OldInputFile.open(InputFileName, ios::in | ios::binary);
            int iii=0;
            while (!OldInputFile.eof())
            {
                if (offset == Item.DataField)
                {
                    getline(OldInputFile, line);
                    cout << "At " << offset << ", record: " << line << endl;
                    break;
                }
                getline(OldInputFile, line);
                int currentLength = line.length();
                offset += currentLength;
            }
            offset += line.length();
            while(!OldInputFile.eof()) {
                if(iii>=ListCount)
                    break;
                getline(OldInputFile, line);
                cout << "At " << offset << ", record: " << line << endl;
                int currentLength = line.length();
                offset += currentLength;
                iii++;
            }
        }
    }
}
