// this file is the utility tool for bruter

#ifndef ARDUINO

// bruter header
#include "../include/bruter.h"


function(brl_std_pkg_name)
{
    hash_set(vm, "package.name", arg_i(0));
    return -1;
}

function(brl_std_pkg_author)
{
    hash_set(vm, "package.author", arg_i(0));
    return -1;
}

function(brl_std_pkg_version)
{
    hash_set(vm, "package.version", arg_i(0));
    return -1;
}

function(brl_std_pkg_description)
{
    hash_set(vm, "package.description", arg_i(0));
    return -1;
}

function(brl_std_pkg_dependency)
{
    Int pdn = hash_find(vm, "package.dependencies.name");
    Int pda = hash_find(vm, "package.dependencies.author");
    Int pdv = hash_find(vm, "package.dependencies.version");
    stack_push(*((IntList*)vm->stack->data[pdn].pointer), arg_i(0));
    stack_push(*((IntList*)vm->stack->data[pda].pointer), arg_i(1));
    stack_push(*((IntList*)vm->stack->data[pdv].pointer), arg_i(2));
    return -1;
}

init(pkg)
{
    register_list(vm, "package.dependencies.name");
    register_list(vm, "package.dependencies.version");
    register_list(vm, "package.dependencies.author");
    register_builtin(vm, "package.name:", brl_std_pkg_name);
    register_builtin(vm, "package.author:", brl_std_pkg_author);
    register_builtin(vm, "package.version:", brl_std_pkg_version);
    register_builtin(vm, "package.description:", brl_std_pkg_description);
    register_builtin(vm, "package.dependency:", brl_std_pkg_dependency);
}

function(brl_std_repo_name)
{
    hash_set(vm, "repository.name", arg_i(0));
    return -1;
}

function(brl_std_repo_url)
{
    hash_set(vm, "repository.url", arg_i(0));
    return -1;
}

function(brl_std_repo_description)
{
    hash_set(vm, "repository.description", arg_i(0));
    return -1;
}

function(brl_std_repo_package)
{
    Int pn = hash_find(vm, "repository.packages.name");
    Int pv = hash_find(vm, "repository.packages.version");
    Int pa = hash_find(vm, "repository.packages.author");
    Int pd = hash_find(vm, "repository.packages.description");
    Int pu = hash_find(vm, "repository.packages.url");
    stack_push(*((IntList*)data(pn).pointer), arg_i(0));
    stack_push(*((IntList*)data(pv).pointer), arg_i(1));
    stack_push(*((IntList*)data(pa).pointer), arg_i(2));
    stack_push(*((IntList*)data(pd).pointer), arg_i(3));
    stack_push(*((IntList*)data(pu).pointer), arg_i(4));
    return -1;
}


init (repo)
{
    register_list(vm, "repository.packages.name");
    register_list(vm, "repository.packages.version");
    register_list(vm, "repository.packages.author");
    register_list(vm, "repository.packages.description");
    register_list(vm, "repository.packages.url");

    register_builtin(vm, "repository.name:", brl_std_repo_name);
    register_builtin(vm, "repository.url:", brl_std_repo_url);
    register_builtin(vm, "repository.description:", brl_std_repo_description);
    register_builtin(vm, "repository.package:", brl_std_repo_package);
}


int main(int argc, char **argv)
{
    Int result = 0;
    StringList *args = make_string_list();
    char* code = NULL;
    char* path = NULL;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0)
        {
            printf("bruter v%s\n", VERSION);
            return 0;
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("bruter v%s\n", VERSION);
            printf("usage: %s [file]\n", argv[0]);
            printf("  -v, --version\t\tprint version\n");
            printf("  -h, --help\t\tprint this help\n");
            printf("  -r, --run [code]\trun code\n");
            printf("  -p, --package [path] [get/set] [name/author/version/description/dependencies] [value]\n");
            printf("  -R, --repository [path] [get/set] [name/url/description/packages] [value]\n");
            return 0;
        }
        else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--run") == 0)
        {
            if (i + 1 < argc)
            {
                code = argv[i + 1];
                i++;
            }
        }
        else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--package") == 0)
        {
            VirtualMachine *vm = make_vm();// init vm
            
            // commented libraries init to not expose std functions and such in pkg usage
            /*// <libraries init>*/
            init_pkg(vm);
            if (i + 3 < argc)
            {
                char* __path;
                // if last char is a / 
                if (argv[i + 1][strlen(argv[i + 1]) - 1] == '/')
                {
                    __path = str_format("%spackage.br", argv[i + 1]);
                }
                else
                {
                    __path = str_format("%s/package.br", argv[i + 1]);
                }

                char* filecontent = readfile(__path);
                if (filecontent == NULL)
                {
                    printf("file %s not found\n", __path);
                    free_vm(vm);
                    return 1;
                }
                
                eval(vm, filecontent, NULL);

                if (strcmp(argv[i + 2], "get") == 0)
                {
                    if (strcmp(argv[i + 3], "name") == 0)
                    {
                        Int index = hash_find(vm, "package.name");
                        if (index != -1)
                        {
                            print_element(vm, index);
                            printf("\n");
                        }
                    }
                    else if (strcmp(argv[i + 3], "author") == 0)
                    {
                        Int index = hash_find(vm, "package.author");
                        if (index != -1)
                        {
                            print_element(vm, index);
                            printf("\n");
                        }
                    }
                    else if (strcmp(argv[i + 3], "version") == 0)
                    {
                        Int index = hash_find(vm, "package.version");
                        if (index != -1)
                        {
                            print_element(vm, index);
                            printf("\n");
                        }
                    }
                    else if (strcmp(argv[i + 3], "description") == 0)
                    {
                        Int index = hash_find(vm, "package.description");
                        if (index != -1)
                        {
                            print_element(vm, index);
                            printf("\n");
                        }
                    }
                    else if (strcmp(argv[i + 3], "dependencies") == 0)
                    {
                        Int pdn = hash_find(vm, "package.dependencies.name");
                        Int pda = hash_find(vm, "package.dependencies.author");
                        Int pdv = hash_find(vm, "package.dependencies.version");
                        IntList *names = (IntList*)data(pdn).pointer;
                        IntList *authors = (IntList*)data(pda).pointer;
                        IntList *versions = (IntList*)data(pdv).pointer;
                        for (Int i = 0; i < names->size; i++)
                        {
                            print_element(vm, names->data[i]);
                            printf(" by ");
                            print_element(vm, authors->data[i]);
                            printf(" version ");
                            print_element(vm, versions->data[i]);
                            printf("\n");
                        }
                    }
                }
                else if (strcmp(argv[i + 2], "set") == 0)
                {
                    if (strcmp(argv[i + 3], "name") == 0)
                    {
                        hash_set(vm, "package.name", new_string(vm, argv[i + 4]));
                    }
                    else if (strcmp(argv[i + 3], "version") == 0)
                    {
                        hash_set(vm, "package.version", new_string(vm, argv[i + 4]));
                    }
                    else if (strcmp(argv[i + 3], "description") == 0)
                    {
                        hash_set(vm, "package.description", new_string(vm, argv[i + 4]));
                    }
                    else if (strcmp(argv[i + 3], "dependency") == 0)
                    {
                        Int pdn = hash_find(vm, "package.dependencies.name");
                        Int pda = hash_find(vm, "package.dependencies.author");
                        Int pdv = hash_find(vm, "package.dependencies.version");
                        stack_push(*((IntList*)data(pdn).pointer), new_string(vm, argv[i + 4]));
                        stack_push(*((IntList*)data(pda).pointer), new_string(vm, argv[i + 5]));
                        stack_push(*((IntList*)data(pdv).pointer), new_string(vm, argv[i + 6]));
                    }
                    // save to file
                    char* text = str_format("package.name: \"%s\";\npackage.author: \"%s\";\npackage.version: \"%s\";\npackage.description: \"%s\";\n", 
                        data(hash_find(vm, "package.name")).string, 
                        data(hash_find(vm, "package.author")).string,
                        data(hash_find(vm, "package.version")).string, 
                        data(hash_find(vm, "package.description")).string);
                    for (Int i = 0; i < ((IntList*)data(hash_find(vm, "package.dependencies.name")).pointer)->size; i++)
                    {
                        text = str_format("%spackage.dependency: \"%s\" \"%s\" \"%s\";\n", text, 
                            data(((IntList*)data(hash_find(vm, "package.dependencies.name")).pointer)->data[i]).string, 
                            data(((IntList*)data(hash_find(vm, "package.dependencies.author")).pointer)->data[i]).string, 
                            data(((IntList*)data(hash_find(vm, "package.dependencies.version")).pointer)->data[i]).string);
                    }
                    writefile(__path, text);
                    free(text);
                }
                free(__path);
            }
            free_vm(vm);
            return 0;
        }
        else if (strcmp(argv[i], "-R") == 0 || strcmp(argv[i], "--repository") == 0)
        {
            VirtualMachine *vm = make_vm();// init vm
            
            // commented libraries init to not expose std functions and such in pkg usage
            /*// <libraries init>*/

            init_repo(vm);
            
            if (i + 3 < argc)
            {
                char* __path = str_format("%s/repository.br", argv[i + 1]);
                char* filecontent = readfile(__path);

                if (filecontent == NULL)
                {
                    printf("file %s not found\n", __path);
                    free_vm(vm);
                    return 1;
                }
                
                eval(vm, filecontent, NULL);

                if (strcmp(argv[i + 2], "get") == 0)
                {
                    if (strcmp(argv[i + 3], "name") == 0)
                    {
                        Int index = hash_find(vm, "repository.name");
                        if (index != -1)
                        {
                            printf("%s", data(index).string);
                            printf("\n");
                        }
                    }
                    else if (strcmp(argv[i + 3], "url") == 0)
                    {
                        Int index = hash_find(vm, "repository.url");
                        if (index != -1)
                        {
                            printf("%s", data(index).string);
                            printf("\n");
                        }
                    }
                    else if (strcmp(argv[i + 3], "description") == 0)
                    {
                        Int index = hash_find(vm, "repository.description");
                        if (index != -1)
                        {
                            printf("%s", data(index).string);
                            printf("\n");
                        }
                    }
                }
                else if (strcmp(argv[i + 2], "url") == 0)
                {
                    if (i + 3 < argc)
                    {
                        IntList *names = (IntList*)data(hash_find(vm, "repository.packages.name")).pointer;
                        Int index = -1;
                        for (Int i = 0; i < names->size; i++)
                        {
                            if (strcmp(data(names->data[i]).string, argv[i + 3]) == 0)
                            {
                                index = i;
                                break;
                            }
                        }

                        if (index != -1)
                        {
                            IntList *urls = (IntList*)data(hash_find(vm, "repository.packages.url")).pointer;
                            print_element(vm, urls->data[index]);
                            printf("\n");
                        }
                    }
                }
                else if (strcmp(argv[i + 2], "set") == 0)
                {
                    if (strcmp(argv[i + 3], "name") == 0)
                    {
                        hash_set(vm, "repository.name", new_string(vm, argv[i + 4]));
                    }
                    else if (strcmp(argv[i + 3], "url") == 0)
                    {
                        hash_set(vm, "repository.url", new_string(vm, argv[i + 4]));
                    }
                    else if (strcmp(argv[i + 3], "description") == 0)
                    {
                        hash_set(vm, "repository.description", new_string(vm, argv[i + 4]));
                    }
                    else if (strcmp(argv[i + 3], "package") == 0)
                    {
                        Int pn = hash_find(vm, "repository.packages.name");
                        Int pv = hash_find(vm, "repository.packages.version");
                        Int pa = hash_find(vm, "repository.packages.author");
                        Int pd = hash_find(vm, "repository.packages.description");
                        Int pu = hash_find(vm, "repository.packages.url");
                        stack_push(*((IntList*)data(pn).pointer), new_string(vm, argv[i + 4]));
                        stack_push(*((IntList*)data(pv).pointer), new_string(vm, argv[i + 5]));
                        stack_push(*((IntList*)data(pa).pointer), new_string(vm, argv[i + 6]));
                        stack_push(*((IntList*)data(pd).pointer), new_string(vm, argv[i + 7]));
                        stack_push(*((IntList*)data(pu).pointer), new_string(vm, argv[i + 8]));
                    }
                    // save to file
                    char* text = str_format("repository.name: \"%s\";\nrepository.url: \"%s\";\nrepository.description: \"%s\";\n", 
                        data(hash_find(vm, "repository.name")).string, 
                        data(hash_find(vm, "repository.url")).string, 
                        data(hash_find(vm, "repository.description")).string);
                    for (Int i = 0; i < ((IntList*)data(hash_find(vm, "repository.packages.name")).pointer)->size; i++)
                    {
                        text = str_format("%srepository.package: \"%s\" \"%s\" \"%s\" \"%s\";\n", text, 
                            data(((IntList*)data(hash_find(vm, "repository.packages.name")).pointer)->data[i]).string, 
                            data(((IntList*)data(hash_find(vm, "repository.packages.author")).pointer)->data[i]).string,
                            data(((IntList*)data(hash_find(vm, "repository.packages.version")).pointer)->data[i]).string,
                            data(((IntList*)data(hash_find(vm, "repository.packages.description")).pointer)->data[i]).string);
                    }
                    writefile(__path, text);
                    free(text);
                }
                free(__path);
            }
        }
        else
        {
            stack_push(*args, argv[i]);
        }
    }
    
    VirtualMachine *vm = make_vm();
    
    // libraries init is not a merely comment
    // <libraries init>

    if (args->size == 0)
    {
        repl(vm);
    }
    else if (args->size == 1) 
    {
        if (code == NULL) // read file pointed by argv[1]
        {
            char *_code = readfile(argv[1]);
            if (_code == NULL)
            {
                printf("file %s not found\n", argv[1]);
                return 1;
            }
            Int filepathindex = new_var(vm);
            // path without file name
            vm->typestack->data[filepathindex] = TYPE_STRING;
            // remove file name
            char *path = stack_shift(*args);
            char *last = strrchr(path, '/');
            if (last == NULL)
            {
                vm->stack->data[filepathindex].string = str_duplicate("");
            }
            else
            {
                vm->stack->data[filepathindex].string = str_nduplicate(path, last - path + 1);
            }
            hash_set(vm, "file.path", filepathindex);
            register_list(vm, "file.args");
            IntList *fileargs = (IntList*)data(hash_find(vm, "file.args")).pointer;
            while (args->size > 0)
            {
                stack_push(*fileargs, new_string(vm, stack_shift(*args)));
            }
            result = eval(vm, _code, NULL);
            free(_code);
        }
        else
        {
            result = eval(vm, code, NULL);
        }
    }
    stack_free(*args);
    free_vm(vm);
    return result;
}

#else
extern "C"
{
    #include "./src/bruter.h"
    
}

Int _ino_serial_begin(VirtualMachine *vm, IntList *args)
{
    Serial.begin((Int)arg(0).number);
    return -1;
}

Int _ino_print(VirtualMachine *vm, IntList *args)
{
    //printf("Type: %d\n", vm->typestack->data[var]);

    Value temp = arg(1);
    Int _type = arg_t(1);
    
    if (_type == TYPE_NUMBER)
    {
        printf("%f\n", temp.number);
    }
    else if (_type == TYPE_STRING)
    {
        printf("%s\n", temp.string);
    }
    else if (_type == TYPE_BUILTIN)
    {
        printf("(function)%p\n", temp.pointer);
    }
    else
    {
        printf("(unknown)\n");
    }
    
    return -1;
}

Int _ino_delay(VirtualMachine *vm, IntList *args)
{
    delay((Int)arg(0).number);
    return -1;
}

Int _ino_millis(VirtualMachine *vm, IntList *args)
{
    return millis();
}

Int _ino_tone(VirtualMachine *vm, IntList *args)
{
    tone((int)arg(0).number, (int)arg(1).number);
    return -1;
}


Int _ino_noTone(VirtualMachine *vm, IntList *args)
{
    noTone((Int)arg(0).number);
    return -1;
}

Int _ino_pinmode(VirtualMachine *vm, IntList *args)
{
    pinMode((Int)arg(0).number, (Int)arg(1).number);
    return -1;
}

Int _ino_digitalwrite(VirtualMachine *vm, IntList *args)
{
    digitalWrite((Int)arg(0).number, (Int)arg(1).number);
    return -1;
}

Int _ino_digitalread(VirtualMachine *vm, IntList *args)
{
    return digitalRead((Int)arg(0).number);
}

Int _ino_analogwrite(VirtualMachine *vm, IntList *args)
{
    analogWrite((Int)arg(0).number, (Int)arg(1).number);
    return -1;
}

Int _ino_analogread(VirtualMachine *vm, IntList *args)
{
    return analogRead((Int)arg(0).number);
}



class Bruter
{
    public:
    void free()
    {
        free_vm(this->vm);
    };
    void registerFunction(char *name, Function func)
    {
        register_builtin(this->vm, name, func);
    };
    Int run(char *str)
    {
        return eval(this->vm, str, NULL);
    };
    VirtualMachine *vm;
    Bruter()
    {
        this->vm = make_vm();
        // <libraries init>
        this->registerFunction((char*)"Serial.begin", _ino_serial_begin);
        this->registerFunction((char*)"Serial.println", _ino_print);
        this->registerFunction((char*)"delay", _ino_delay);
        this->registerFunction((char*)"millis", _ino_millis);
        this->registerFunction((char*)"tone", _ino_tone);
        this->registerFunction((char*)"noTone", _ino_noTone);
        this->registerFunction((char*)"pinMode", _ino_pinmode);
        this->registerFunction((char*)"digitalWrite", _ino_digitalwrite);
        this->registerFunction((char*)"digitalRead", _ino_digitalread);
        this->registerFunction((char*)"analogWrite", _ino_analogwrite);
        this->registerFunction((char*)"analogRead", _ino_analogread);
    }
};

Bruter *session = new Bruter();

void setup()
{
    Serial.begin(115200);
    session->run((char*)"hash.set 'str' (@@looping);");
    session->run((char*)"Serial.println (gindex str);");
    session->run((char*)"Serial.println (gindex str);");
}

void loop()
{
    delay(1000);

    session->run((char*)"Serial.println (gindex str);");
}

#endif