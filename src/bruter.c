#include "bruter.h"

//var.c
//string.c

void freeVM(VirtualMachine *vm)
{
    for (Int i = 0; i < vm->stack->size; i++)
    {
        if (vm->stack->data[i] != NULL)
        {
            freeVar(vm, i);
        }
    }
    while (vm->hashes->size > 0)
    {
        Hash *temp = StackPop(*vm->hashes);
        free(temp->key);
        temp->key = NULL;
        free(temp);
        temp = NULL;
    }
    StackFree(*vm->stack);
    StackFree(*vm->hashes);
    StackFree(*vm->empty);
    free(vm);
    vm = NULL;
}

// Parser functions
// idea: parse could use indexes of the args on the stack instead of the args themselves 
IntList* parse(VirtualMachine *vm, char *cmd) 
{
    IntList *result = makeIntList();
    StringList *splited = specialSplit(cmd);
    while (splited->size > 0) 
    {
        char* str = StackShift(*splited);
        if (str[0] == '(') 
        {
            Function __eval = (Function)vm->stack->data[hashfind(vm, "eval")]->value.pointer;
            
            char* _str = strndup(str + 1, strlen(str) - 2);
            //free(_str);

            IntList *args = makeIntList();
            StackPush(*args, newString(vm, _str));
            Int res = __eval(vm, args);
            if (res == -1) 
            {
                StackPush(*result, newError(vm, "Error in eval"));
            } 
            else 
            {
                StackPush(*result, res);
            }
            //freeVar(vm, res);
            free(_str);
            _str = NULL;
            StackFree(*args);
        }
        else if (str[0] == '!' && str[1] == '(') // literal string
        {
            char* _str = strndup(str + 2, strlen(str) - 3);
            StackPush(*result, newString(vm, _str));
            free(_str);
            _str = NULL;
        }
        else if (str[0] == '@') 
        {
            if (strlen(str) == 1) 
            {
                StackPush(*result, newError(vm, "Variable name not found"));
            }
            else if (str[1] == '(') 
            {
                Function __eval = (Function)vm->stack->data[hashfind(vm, "eval")]->value.pointer;
                char* _str = strndup(str + 2, strlen(str) - 3);
                IntList *args = makeIntList();
                StackPush(*args, newString(vm, _str));
                Int res = __eval(vm, args);
                if (res == -1) 
                {
                    StackPush(*result, newError(vm, "Error in eval"));
                } 
                else 
                {
                    StackPush(*result, res);
                    res = __eval(vm, args);
                    if (res == -1) 
                    {
                        StackPush(*result, newError(vm, "Error in eval"));
                    } 
                    else 
                    {
                        StackPush(*result, res);
                    }
                }
                free(_str);
                _str = NULL;
                StackFree(*args);
            }
            else if(str[1] == '@') // @@varname, get variable @varname and use its result as the key to get the correct variable
            {
                Int index = hashfind(vm, str + 2);
                Variable *var1 = vm->stack->data[index];
                Int index2 = hashfind(vm, var1->value.string);

                if (index2 == -1) 
                {
                    StackPush(*result, newError(vm, "Variable not found"));
                } 
                else 
                {
                    StackPush(*result, index2);
                }
            }
            else if(str[1] >= '0' && str[1] <= '9') 
            {
                StackPush(*result, atoi(str + 1));
            }
            else if(str[1] == '!' && str[2] == '(')
            {
                char * _str = strndup(str + 3, strlen(str) - 4);
                Int index = hashfind(vm, _str);
                if (index == -1) 
                {
                    StackPush(*result, newError(vm, "Variable not found"));
                } 
                else 
                {
                    StackPush(*result, index);
                }
                free(_str);
                _str = NULL;
            }
            else
            {
                Int index = hashfind(vm, str + 1);

                if (index == -1) 
                {
                    StackPush(*result, newError(vm, "Variable not found"));
                } 
                else 
                {
                    Int var = newVar(vm);
                    setVar(vm, var, vm->stack->data[index]->type, vm->stack->data[index]->value, Nil);
                    StackPush(*result, var);
                }
            }
        }
        else if ((str[0] >= '0' && str[0] <= '9') || str[0] == '-') 
        {
            StackPush(*result, newNumber(vm, atof(str)));
        } 
        else //string 
        {
            StackPush(*result, newString(vm, str));
        }

        free(str);
        str = NULL;
    }
    StackFree(*splited);
    return result;
}

Int interpret(VirtualMachine *vm, char* str) 
{    
    Int result;
    IntList *args = parse(vm, str);

    Reference funcname = refget(vm, StackShift(*args));

    if (funcname.variable->type == TYPE_ERROR) 
    {
        printf("%s\n", funcname.variable->value.string);
        return -1;
    }

    if (funcname.variable->type != TYPE_STRING) 
    {
        printf("First argument must be a string\n");
        return -1;
    }

    Int index = hashfind(vm, funcname.variable->value.string);
    if (index == -1 ) 
    {
        printf("Function not found\n");
        return -1;
    }

    if (vm->stack->data[index] == NULL) 
    {
        printf("function seems null\n");
        return -1;
    }

    if (vm->stack->data[index]->type != TYPE_FUNCTION) 
    {
        printf("Variable is not a function\n");
        return -1;
    }

    Function func = (Function)vm->stack->data[index]->value.pointer;
    result = func(vm, args);
    
    freeref(vm, funcname);
    StackFree(*args);

    return result;
}

Int eval(VirtualMachine *vm, char *str)
{
    StringList *splited = splitString(str, ";");
    Int result = -1;
    while (splited->size > 0)
    {
        char *cmd = StackShift(*splited);
        result = interpret(vm, cmd);
        free(cmd);
        cmd = NULL;
        if (result != -1)
        {
            break;
        }
    }
    StackFree(*splited);
    return result;
}

char *read_file(const char *filename) 
{
    FILE *file = fopen(filename, "r");
    if (!file) 
    {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(length + 1);
    if (!content) 
    {
        fclose(file);
        return NULL;
    }

    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
}

int write_file(const char *filename, const char *content) 
{
    FILE *file = fopen(filename, "w");
    if (!file) 
    {
        return -1;
    }

    size_t length = strlen(content);
    size_t written = fwrite(content, 1, length, file);
    
    fclose(file);
    return (written == length) ? 0 : -1;
}

//main
//main
void main()
{
    VirtualMachine *vm = makeVM();
    
    initStd(vm);

    char* cmd = "set a 50;"
                "set b 100;"
                "set c 150;"
                "set d 200;"
                "set e 250;"
                "set f 300;"
                "set g 350;"
                "set h 400;"
                "set i 450;"
                "set j 500;"
                "set k 550;"
                "set l 600;"
                "set lst (list);"
                "set abc dasdasdasd;";

    eval(vm, cmd);
    // free
    freeVM(vm);
}