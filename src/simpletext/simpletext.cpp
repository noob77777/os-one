#include <simpletext/simpletext.h>

namespace simpletext
{
    static editor_state *state_ = nullptr;
    static FileSystem *fs_ = nullptr;
    const uint8_t THEME = 0x75;

    void render_msg(const char *msg)
    {
        int x = 0;
        int y = HEIGHT - 1;

        for (int i = 0; i < WIDTH; ++i)
        {
            VIDEO_MEMORY[2 * (WIDTH * y + i)] = ' ';
            VIDEO_MEMORY[2 * (WIDTH * y + i) + 1] = THEME;
        }

        for (int i = 0; state_->filename[i] != '\0'; ++i)
        {
            VIDEO_MEMORY[2 * (WIDTH * y + x)] = state_->filename[i];
            VIDEO_MEMORY[2 * (WIDTH * y + x) + 1] = THEME;
            x++;
        }

        VIDEO_MEMORY[2 * (WIDTH * y + x)] = ':';
        x++;
        VIDEO_MEMORY[2 * (WIDTH * y + x)] = ' ';
        x++;

        for (int i = 0; msg[i] != '\0'; ++i)
        {
            VIDEO_MEMORY[2 * (WIDTH * y + x)] = msg[i];
            VIDEO_MEMORY[2 * (WIDTH * y + x) + 1] = THEME;
            x++;
        }
    }

    void render_header()
    {
        int x = 32;
        int y = 0;

        for (int i = 0; i < WIDTH; ++i)
        {
            VIDEO_MEMORY[2 * (WIDTH * y + i)] = ' ';
            VIDEO_MEMORY[2 * (WIDTH * y + i) + 1] = THEME;
        }

        const char *str = "SimpleText v1.0";

        for (int i = 0; str[i] != '\0'; ++i)
        {
            VIDEO_MEMORY[2 * (WIDTH * y + x)] = str[i];
            VIDEO_MEMORY[2 * (WIDTH * y + x) + 1] = THEME;
            x++;
        }
    }

    /*
     * Optimization required
     */
    void render()
    {
        // state_ptr is always not null if render is called
        display::clear();

        render_header();

        int number_of_lines = 0;
        int idx = 0;
        uint8_t flag = 0;
        for (int i = 0; state_->buffer[i] != '\0'; i++)
        {
            if (flag == 0 && number_of_lines == state_->line)
            {
                flag = 1;
                idx = i;
            }
            if (state_->buffer[i] == '\n')
            {
                number_of_lines++;
            }
        }

        int x = 0;
        int y = 1;
        for (int i = idx; state_->buffer[i] != '\0'; ++i)
        {
            if (i == state_->cursor_idx)
            {
                VIDEO_MEMORY[2 * (WIDTH * y + x)] = '_';
                x++;
            }

            switch (state_->buffer[i])
            {
            case '\n':
                x = 0;
                y++;
                break;
            case '\t':
                x += 4;
                break;
            default:
                VIDEO_MEMORY[2 * (WIDTH * y + x)] = state_->buffer[i];
                x++;
                break;
            }

            if (x >= SimpleText::WINDOW_WIDTH)
            {
                x = 0;
                y++;
            }

            if (y >= SimpleText::WINDOW_HEIGHT)
            {
                break;
            }
        }

        render_msg("editing");
    }

    int init(editor_state *state, int fd, FileSystem *fs)
    {
        display::clear();
        state->fd = fd;
        state->cursor_idx = 0;
        state->line = 0;
        state->buffer = new char[FileSystem::CLUSTER_SIZE];
        if (state->buffer == nullptr)
        {
            return 1;
        }

        int status = fs->read(fd, (uint8_t *)state->buffer, FileSystem::CLUSTER_SIZE);
        if (status == FileSystem::ERROR)
        {
            delete state->buffer;
            return 1;
        }

        state_ = state;
        fs_ = fs;
        return 0;
    }

    int save()
    {
        int status = fs_->write(state_->fd, (uint8_t *)state_->buffer, FileSystem::CLUSTER_SIZE);
        return status;
    }

    void close(editor_state *state)
    {
        delete state->buffer;
        display::clear();
    }

    /*
     * Function not optimized
     */
    void keyboard_handler(char chr)
    {
        if (chr == '\a')
        {
            if (save() == 0)
                render_msg("saved");
        }
        else
        {
            if (chr == UP)
            {
                int number_of_lines = 0;
                int current_line_number = 0;
                int prev_end = 0;
                for (int i = 0; state_->buffer[i] != '\0'; i++)
                {
                    if (i == state_->cursor_idx)
                    {
                        current_line_number = number_of_lines;
                        state_->cursor_idx = prev_end;
                        if (current_line_number > 0)
                        {
                            current_line_number--;
                            if (current_line_number < state_->line)
                            {
                                state_->line--;
                            }
                        }
                        break;
                    }
                    if (state_->buffer[i] == '\n')
                    {
                        prev_end = i;
                        number_of_lines++;
                    }
                }
            }
            else if (chr == DOWN)
            {
                int number_of_lines = 0;
                int current_line_number = 0xFF;
                int prev_end = 0;
                for (int i = 0; state_->buffer[i] != '\0'; i++)
                {
                    if (i == state_->cursor_idx)
                    {
                        current_line_number = number_of_lines;
                    }
                    if (state_->buffer[i] == '\n')
                    {
                        if (number_of_lines == current_line_number + 1)
                        {
                            state_->cursor_idx = i;
                            if (number_of_lines - state_->line + 1 >= SimpleText::WINDOW_HEIGHT)
                            {
                                state_->line++;
                            }
                            break;
                        }
                        number_of_lines++;
                    }
                }
            }
            else if (chr == RIGHT)
            {
                if (state_->buffer[state_->cursor_idx] != '\0')
                {
                    state_->cursor_idx++;
                }
            }
            else if (chr == LEFT)
            {
                if (state_->cursor_idx != 0)
                {
                    state_->cursor_idx--;
                }
            }
            else if (chr == '\b')
            {
                if (state_->cursor_idx != 0)
                {
                    int eof = 0;
                    for (int i = state_->cursor_idx; state_->buffer[i] != '\0'; i++)
                        eof = i;
                    for (int i = state_->cursor_idx - 1; state_->buffer[i] != '\0'; i++)
                        state_->buffer[i] = state_->buffer[i + 1];
                    state_->buffer[eof] = '\0';
                    state_->cursor_idx--;
                }
            }
            else
            {
                if (state_->buffer[state_->cursor_idx] == '\0')
                {
                    state_->buffer[state_->cursor_idx++] = chr;
                    state_->buffer[state_->cursor_idx] = '\0';
                }
                else
                {
                    int eof = 0;
                    for (int i = state_->cursor_idx; state_->buffer[i] != '\0'; i++)
                    {
                        eof = i + 1;
                    }
                    for (int i = eof; i >= state_->cursor_idx; i--)
                    {
                        state_->buffer[i + 1] = state_->buffer[i];
                    }
                    state_->buffer[state_->cursor_idx++] = chr;
                }
            }
            render();
        }
    }
} // namespace simpletext

SimpleText::SimpleText(FileSystem *fs, KeyboardDriver *keyboard) : Program("simpletext")
{
    this->fs = fs;
    this->keyboard = keyboard;
}

int SimpleText::main(int argc, char *const argv[], char *const env[])
{
    if (argc == 1)
    {
        kprintf("simpletext: missing file operand\n");
        return 0;
    }
    if (argc >= 3)
    {
        kprintf("simpletext: too many files as argument\n");
        return 0;
    }

    const char *filename = argv[1];
    int fd = fs->open(filename);
    if (fd == FileSystem::ERROR)
    {
        kprintf("simpletext: filesystem error\n");
        return 1;
    }

    this->state.filename = filename;
    if (simpletext::init(&this->state, fd, fs))
    {
        kprintf("simpletext: memory allocation error or filesystem error\n");
        return 1;
    }
    keyboard->on_key_press(simpletext::keyboard_handler);

    simpletext::render();

    for (;;)
    {
        if (keyboard->SIGINT)
        {
            simpletext::close(&this->state);
            keyboard->on_key_press(terminal::keyboard_handler);
            keyboard->SIGINT = 0;
            break;
        }
    }

    return 0;
}
