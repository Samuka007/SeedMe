#ifndef SeedDB_hpp
#define SeedDB_hpp

#include "./dataframe/DataPre.hpp"
#include "./userframe/UserPre.hpp"
#include "./metadata/Metadata.hpp"

class SeedDB : DataPre, UserPre, Metadata {
    public:
        SeedDB(std::string filename)
        :   DataPre(filename+".data"s), 
            UserPre(filename+".user"s),
            Metadata(filename+".meta"s)
        {}

        namespace UserControl{
            void CreateUser();

            void UpdateUser();

            void GetUserSource();

            void DeleteUser();
        }

        namespace SrcControl{
            void CreateSource();

            void GetSource();

            void UpdateSource();

            void DeleteSource();
        }        

        /**
        * store
        * read
        * generate 
        * change metadata
        * 
        * log metadata and store source
        * 
        */

        /*
        *   two filters:
        *       - userid                    read    write
        *           - 0 = visitor           all     0
        *           - 1 = suporior          all     all
        *           - 100+ = user           all     1
        *       - tags(metadatas)
        */
}

#endif