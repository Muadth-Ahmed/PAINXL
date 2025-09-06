using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

class EmlInterpreter
{
    const int TAPE_SIZE = 300000;
    const int MAX_COMMANDS = 10000000;

    static void Main(string[] args)
    {
        if (args.Length < 1)
        {
            Console.WriteLine("Usage: EmlInterpreter <filename.eml>");
            return;
        }

        string filename = args[0];
        if (!File.Exists(filename))
        {
            Console.WriteLine("Error: File not found.");
            return;
        }

        List<string> commands = new List<string>();
        try
        {
            string content = File.ReadAllText(filename);
            StringBuilder bits = new StringBuilder();

            foreach (char c in content)
            {
                if (c == '0' || c == '1')
                    bits.Append(c);
            }

            for (int i = 0; i + 7 < bits.Length; i += 8)
            {
                commands.Add(bits.ToString(i, 8));
                if (commands.Count >= MAX_COMMANDS)
                {
                    Console.WriteLine("Too many commands!");
                    return;
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine("Error reading file: " + ex.Message);
            return;
        }

        byte[] tape = new byte[TAPE_SIZE];
        int ptr = 0;

        for (int pc = 0; pc < commands.Count; pc++)
        {
            string cmd = commands[pc];

            switch (cmd)
            {
                case "01001001": // >
                    ptr++;
                    if (ptr >= TAPE_SIZE)
                    {
                        Console.WriteLine("Pointer out of bounds (right)");
                        return;
                    }
                    break;

                case "10101001": // <
                    if (ptr <= 0)
                    {
                        Console.WriteLine("Pointer out of bounds (left)");
                        return;
                    }
                    ptr--;
                    break;

                case "00100101": // +
                    tape[ptr]++;
                    break;

                case "10010101": // -
                    tape[ptr]--;
                    break;

                case "11100001": // .
                    Console.Write((char)tape[ptr]);
                    break;

                case "00011101": // ,
                    int input = Console.Read();
                    tape[ptr] = input == -1 ? (byte)0 : (byte)input;
                    break;

                case "00101101": // clear cell
                    tape[ptr] = 0;
                    break;

                case "00000001": // loop start
                    if (tape[ptr] == 0)
                    {
                        int depth = 1;
                        while (depth > 0)
                        {
                            pc++;
                            if (pc >= commands.Count)
                            {
                                Console.WriteLine("Unmatched loop start (00000001)");
                                return;
                            }
                            if (commands[pc] == "00000001") depth++;
                            else if (commands[pc] == "11111101") depth--;
                        }
                    }
                    break;

                case "11111101": // loop end
                    if (tape[ptr] != 0)
                    {
                        int depth = 1;
                        while (depth > 0)
                        {
                            pc--;
                            if (pc < 0)
                            {
                                Console.WriteLine("Unmatched loop end (11111101)");
                                return;
                            }
                            if (commands[pc] == "11111101") depth++;
                            else if (commands[pc] == "00000001") depth--;
                        }
                        pc--;
                    }
                    break;

                default:
                    Console.WriteLine("Unknown command: " + cmd);
                    return;
            }
        }
    }
}
