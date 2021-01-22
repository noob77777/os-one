#include <sys/terminal.h>

namespace terminal
{
    ProgramManager *program_manager = nullptr;

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
        else if (chr == '\n')
        {
            kprintf("\n");
            if (strlen(terminal_buffer) != 0)
            {
                int num_tokens = 0;
                char **tokens = strtoken(terminal_buffer, ' ', &num_tokens);

                Program *ps = (tokens != nullptr) ? program_manager->program(tokens[0]) : nullptr;
                if (ps == nullptr)
                {
                    if (tokens)
                    {
                        kprintf(tokens[0]);
                        kprintf(": command not found\n");
                    }
                }
                else
                {
                    ps->main(num_tokens, tokens, nullptr);
                }

                if (tokens != nullptr)
                {
                    for (int i = 0; i < num_tokens; i++)
                    {
                        MemoryManager::free(tokens[i]);
                    }
                    MemoryManager::free(tokens);
                }
            }
            kprintf("root@host: ");
            idx = 0;
            terminal_buffer[idx] = 0;
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
            break;
        }
    }

    return 0;
}
