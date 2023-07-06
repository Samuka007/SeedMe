#include "Seedme.hpp"

int main( int argc, char* argv[] ){
    try{Seedme db("filename");}catch(...){}
    
    return 0;
}