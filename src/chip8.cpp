#include "include/chip8.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

void Chip8::initialize()
{

    pc = 0x200;
    opcode = 0;
    index = 0;
    sp = 0;

    for (int i = 0; i < 80; i++)
    {
        memory[i] = fontSet[i];
    }
}
void Chip8::loadGame()
{

    ifstream gFile("src/pong2.c8", ios::in | ios::binary);
    if (gFile.is_open())
    {
        //const int buffSize=
        //string buffer;

        streampos begin, end;
        begin = gFile.tellg();
        gFile.seekg(0, ios::end);
        end = gFile.tellg();
        int bufferSize = end - begin;
        char buffer[bufferSize];
        gFile.read(buffer, bufferSize);
        for (int i = 0; i < bufferSize; i++)
        {
            memory[i + 512] = buffer[i];
        }
    }
    gFile.close();
}
void Chip8::setDelayTimer(Uchar &t)
{
    delayTimer = t;
}

Uchar Chip8::getDelayTimer()
{
    return delayTimer;
}

void Chip8::setSoundTimer(Uchar &t)
{
    soundTimer = t;
}

Uchar Chip8::getSoundTimer()
{
    return soundTimer;
}

void Chip8::emulateCycle()
{

    opcode = memory[pc] << 8 | memory[pc + 1];
    Ushort fetchOpcode = opcode & 0xF000;
    if (fetchOpcode >= 0x1000 && fetchOpcode != 0x8000 && fetchOpcode != 0xF000)
    {
        switch (fetchOpcode)
        {
        case 0x1000:
            pc = opcode & 0x0FFF;
            break;
        case 0x2000:
            stack[sp] = pc;
            sp++;
            pc = opcode & 0xFFF;
            break;
        case 0x3000:
            if (v[opcode & 0x0F00 >> 8] == (opcode & 0x00FF))
                pc += 4;
            break;
        case 0x4000:
            if (v[opcode & 0x0F00 >> 8] != (opcode & 0x00FF))
                pc += 4;
            break;
        case 0x5000:
            if (v[opcode & 0x0F00 >> 8] == v[opcode & 0x00F0 >> 4])
                pc += 4;
            break;
        case 0x6000:
            v[opcode & 0x0F00 >> 8] = opcode & 0x00FF;
            pc += 2;
            break;
        case 0x7000:
            v[opcode & 0x0F00 >> 8] += opcode & 0x00FF;
            pc += 2;
            break;
        case 0x9000:
            if (v[opcode & 0x0F00 >> 8] != v[opcode & 0x00F0 >> 4])
                pc += 4;
            break;
        case 0xA000:
            index = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0xB000:
            pc = v[0x0] + (opcode & 0x0FFF);
            break;
        case 0xC000:
            srand((unsigned)time(0));
            v[opcode & 0x0F00 >> 8] = (rand() % 255) & (opcode & 0x00FF);
            pc += 2;
            break;
        case 0xD000: //ToDo
            pc += 2;
            break;
        case 0xE000:
            if ((opcode & 0x000F) == 0x000E) //ToDo
            {
            }
            else //ToDo
            {
            }
            pc += 2;
            break;

        default:
            break;
        }
    }
    else if (fetchOpcode == 0x8000)
    {
        Ushort lastNipple = opcode & 0x000F;
        switch (lastNipple)
        {
        case 0x0000:
            v[opcode & 0x0F00 >> 8] = v[opcode & 0x00F0 >> 4];
            pc += 2;
            break;
        case 0x0001:
            v[opcode & 0x0F00 >> 8] = v[opcode & 0x0F00 >> 8] | v[opcode & 0x00F0 >> 4];
            pc += 2;
            break;
        case 0x0002:
            v[opcode & 0x0F00 >> 8] = v[opcode & 0x0F00 >> 8] & v[opcode & 0x00F0 >> 4];
            pc += 2;
            break;
        case 0x0003:
            v[opcode & 0x0F00 >> 8] = v[opcode & 0x0F00 >> 8] ^ v[opcode & 0x00F0 >> 4];
            pc += 2;
            break;
        case 0x0004:
            if ((v[opcode & 0x0F00 >> 8] + v[opcode & 0x00F0 >> 4]) > 0xFF)
            {
                v[0xF] = 0x01;
            }
            else
            {
                v[0xF] = 0x01;
            }
            v[opcode & 0x0F00 >> 8] += v[opcode & 0x00F0 >> 4];
            pc += 2;
            break;

            //Vx-=Vy, v[f]=0, if no borrow and v[f]=1 ,if there is a borrow
        case 0x0005:
            if ((v[opcode & 0x0F00 >> 8] & 0x0F) >= (v[opcode & 0x00F0 >> 4] & 0x0F))
            {
                v[0xF] = 0x01;
            }
            else
            {
                v[0xF] = 0x00;
            }
            v[opcode & 0x0F00 >> 8] -= v[opcode & 0x00F0 >> 4];
            pc += 2;
            break;
        case 0x0006:
            v[0xF] = (opcode & 0x0100 >> 8);
            v[opcode & 0x0F00 >> 8] >>= 1;
            pc += 2;
            break;
        case 0x0007:
            if ((v[opcode & 0x0F00 >> 8] & 0x0F) <= (v[opcode & 0x00F0 >> 4] & 0x0F))
            {
                v[0xF] = 0x01;
            }
            else
            {
                v[0xF] = 0x00;
            }
            v[opcode & 0x0F00 >> 8] = v[opcode & 0x00F0 >> 4] - v[opcode & 0x0F00 >> 8];
            pc += 2;
            break;
        case 0x000E:
            v[0xF] = (opcode & 0x0800 >> 8);
            v[opcode & 0x0F00 >> 8] <<= 1;
            pc += 2;
            break;

        default:
            break;
        }
    }
    else if (fetchOpcode == 0xF000)
    {
        Ushort lastByte = opcode & 0x00FF;

        switch (lastByte)
        {
        case 0x0007:
            v[opcode & 0x0F00 >> 8] = getDelayTimer();
            pc += 2;
            break;

        default:
            break;
        }
    }
}