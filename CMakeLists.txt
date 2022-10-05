cmake_minimum_required(VERSION 3.15)

project(ast)

set(CMAKE_C_STANDARD 11)

include_directories(include)


add_executable(ast 
        main.c
        format/format.c
        lex/lex.c
        parser/parser.c
        stack/stack.c
        token/token.c
        symboltable/symboltable.c
        )