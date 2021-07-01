from generation.decode_builder import add_decode


def main():
    src = open("data/input_header.h").readlines()
    src = add_decode(src)
    with open("bench/mappings.h", "w") as o:
        for line in src:
            o.write(line)

if __name__ == "__main__":
    main()
