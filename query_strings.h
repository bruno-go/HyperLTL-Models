#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>
#include <iostream>

class Qwriter{
public:
    std::ofstream& out;
    std::vector<std::string> stack;
    int ident = 0;

    Qwriter(std::ofstream& _out) : out(_out) {};

    void write_ident(){
        for(int i = 0; i < ident; i++)
            out << " ";
    }

    void write(std::string line){
        write_ident();
        out << line << std::endl;
    }

    void iwrite_line(std::string line){
        write(line);
        ident++;

        constexpr char whitespace[] = " \n\t\r\v\f";

        auto end = line.find_first_of(whitespace);

        if(end != std::string::npos){
            if(line[end] != '>'){
                line[end] = '>';
            }
        }
    
        std::string closed_tag = "</" + line.substr(1, end);
        stack.push_back(closed_tag);
    }

    void close(int count = 1){
        for(int i = 0; i < count; i++){
            std::string line = stack.back();
            ident--;
            write(line);

            stack.pop_back();
            
        }
    }

    void close_all(){
        close(stack.size());
    }

};

class Query{
public:
    static const inline std::string header = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    static const inline std::string property_set =  "<property-set xmlns=\"http://tapaal.net/\">";

    static std::string open(std::string str){
        return "<" + str + ">";
    }

    static std::string exists_path(int i){
        std::stringstream buf;
        buf << "<exists-path name=" << "\"T" << i << "\">";
        return buf.str();
    }

    static std::string path_scope(int i){
        std::stringstream buf;
        buf << "<path-scope name=" << "\"T" << i << "\">";
        return buf.str();
    }

    static std::string place(std::string name){
        return "<place>" + name + "</place>";
    }

    static std::string id(std::string name){
        return "<id>" + name + "</id>";
    }

     static std::string description(std::string desc){
        return "<description>" + desc + "</description>";
    }

    static std::string int_constant(int i){
        std::stringstream buf;
        buf << "<integer-constant>" << i << "</integer-constant>";
        return buf.str();
    }

    Query() = delete;
};