# TODO

- extern types should be in the TypeCache with nullptr for the value (indicating they're extern types)
- the alias name in a TypeAlias should also be entered in the TypeCache


- implement test coverage showing case values don't overflow the switching type.
- implement test coverage showing the switching type is an integer type. 

- implement an AST validator as tree automata. It will validate relationships only, i.e. attribute block needs to be appended to an attribute node, EnumField are only under Enum, etc. This will be one of the "standard" plugins available for users, it will be distributed with the tool.
- implement a print AST plugin, it will be a "standard" plugin distributed with the tool.
- open all unit tests and review headers to ensure I've included only things we're using. 

## Later 

- implement initialization lists so arrays can be initialized