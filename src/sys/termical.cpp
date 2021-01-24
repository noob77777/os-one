#include <sys/terminal.h>

namespace terminal
{
    ProgramManager *program_manager = nullptr;
    Program *ps = nullptr;
    char **tokens = nullptr;
    int num_tokens = 0;

    void keyboard_handler(char chr)
    {
        static char terminal_buffer[80] = "";
        static int idx = 0;
        if (chr == '\b')
        {
            if (idx > 0)
            {
                idx--;
                terminal_buffer[idx] = 0;
                display::backspace();
            }
        }
        else if (chr == UP || chr == DOWN || chr == RIGHT || chr == LEFT)
        {
            ;
        }
        else if (chr == '\n')
        {
            kprintf("\n");
            if (strlen(terminal_buffer) != 0)
            {
                num_tokens = 0;
                tokens = strtoken(terminal_buffer, ' ', &num_tokens);

                ps = (tokens != nullptr) ? program_manager->program(tokens[0]) : nullptr;

                if (ps == nullptr)
                {
                    if (tokens)
                    {
                        kprintf(terminal::tokens[0]);
                        kprintf(": command not found\n");
                    }
                }
            }
            idx = 0;
            terminal_buffer[idx] = 0;
            kprintf("root@host: ");
        }
        else
        {
            const char string[2] = {chr, '\0'};
            kprintf(string);
            terminal_buffer[idx++] = chr;
            idx = idx % 80;
            terminal_buffer[idx] = '\0';
        }
    }
} // namespace terminal

Terminal::Terminal(ProgramManager *program_manager, KeyboardDriver *keyboard) : Program("terminal")
{
    this->keyboard = keyboard;
    terminal::program_manager = program_manager;
    keyboard->on_key_press(terminal::keyboard_handler);
}

int Terminal::main(int argc, char *const argv[], char *const env[])
{
    kprintf("root@host: ");

    for (;;)
    {
        if (keyboard->SIGINT)
        {
            kprintf("\n^C\n");
            keyboard->reset();
            keyboard->SIGINT = 0;
            break;
        }

        if (terminal::ps != nullptr)
        {

            terminal::ps->main(terminal::num_tokens, terminal::tokens, nullptr);
            terminal::ps = nullptr;

            if (terminal::tokens != nullptr)
            {
                for (int i = 0; i < terminal::num_tokens; i++)
                {
                    MemoryManager::free(terminal::tokens[i]);
                }
                MemoryManager::free(terminal::tokens);
            }

            kprintf("root@host: ");
        }
    }

    return 0;
}
