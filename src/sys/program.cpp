#include <sys/program.h>

Program::Program(const char *name)
{
    this->name = name;
}

int Program::main(int argc, char *const argv[], char *const env[])
{
    return 0;
}

const char *Program::get_name()
{
    return this->name;
}

ProgramManager::ProgramManager()
{
    ptr = 0;
}

void ProgramManager::add_program(Program *program)
{
    ps[ptr++] = program;
}

void ProgramManager::start()
{
    ps[0]->main(0, nullptr, nullptr);
}

Program *ProgramManager::program(const char *name)
{
    for (int i = 0; i < ptr; i++)
        if (strcmp(ps[i]->get_name(), name))
            return ps[i];

    return nullptr;
}
