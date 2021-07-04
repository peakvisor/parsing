def get_decoder(s):
    start = s.find("<") + 1
    end = s.find(",", start)
    return s[start: end]

def get_speed(s):
    start = s.find("=") + 1
    end = s.find("/s", start)
    speed = float(s[start:end-1])
    if s[end-1] == "k":
        speed /= 1000
    else:
        assert(s[end-1] == "M")
    return speed

def report(src):
    decoders = dict()

    count = 0
    for line in src:
        name = get_decoder(line)
        if name not in decoders:
            decoders[name] = []
        decoders[name].append((get_speed(line), count))

        count += 1
    decoders = [(min([x[1] for x in v]), k, v) for k, v in decoders.items()]
    decoders.sort(key=lambda x: x[0])

    for d in decoders:
        d[2].sort(key=lambda x: x[1])
        speeds = [x[0] for x in d[2]]
        speed_string = str(speeds[0])[:5] + "M"
        for i in range(1, len(speeds)):
            speed_string += "\t"
            diff = 100 * speeds[i] / speeds[i-1] - 100
            diff = "+" + str(diff) if diff > 0 else str(diff)
            diff = diff[:5]
            speed_string += str(diff) + "%"
            speed_string += "\t" + str(speeds[i])[:5] + "M"

        print(d[1] + "\t" + speed_string)
