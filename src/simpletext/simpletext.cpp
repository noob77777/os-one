#include <simpletext/simpletext.h>

namespace simpletext
{
    static editor_state *state_ = nullptr;
    static FileSystem *fs_ = nullptr;

    void render_msg(const char *msg)
    {
        int x = 0;
        int y = HEIGHT - 1;

        for (int i = 0; i < WIDTH; ++i)
        {
            VIDEO_MEMORY[2 * (WIDTH * y + i)] = ' ';
        }

        for (int i = 0; state_->filename[i] != '\0'; ++i)
        {
            VIDEO_MEMORY[2 * (WIDTH * y + x)] = state_->filename[i];
            x++;
        }

        VIDEO_MEMORY[2 * (WIDTH * y + x)] = ':';
        x++;
        VIDEO_MEMORY[2 * (WIDTH * y + x)] = ' ';
        x++;

        for (int i = 0; msg[i] != '\0'; ++i)
        {
            VIDEO_MEMORY[2 * (WIDTH * y + x)] = msg[i];
            x++;
        }
    }

    void render()
    {
        // state_ptr is always not null if render is called
        render_msg("editing");
    }

    int init(editor_state *state, int fd, FileSystem *fs)
    {
        display::clear();
        state->fd = fd;
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

    void keyboard_handler(char chr)
    {
        if (chr == '\a')
        {
            if (save() == 0)
                render_msg("saved");
        }
        else
        {
            
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
