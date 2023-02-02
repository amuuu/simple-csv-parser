# Simple CSV Parser (C++)

Howdy 🤠. This is a simple CSV parser.

## Sample usage

```cpp
ParserSettings settings{};
settings.ignoredFirstRowsCount = 2;
settings.SetConvertorMethod<int>([](const std::string& token) { return std::stoi(token); });
settings.SetConvertorMethod<std::string>([](const std::string& token) { return token; });

Parser<std::string, int> parser{ "Source/simple.csv", settings };

parser.Parse();

std::string a{}; int b{};
for (int r = 0; r < parser.GetRowCount(); r++)
{
    parser.GetRowData<std::string, int>(r, a, b);
    std::cout << "a: " << a << " b: " << b << std::endl;
}
```

## TODO
- [ ] Parallelize
- [ ] Special cases for single and double quotes
- [ ] Exception handling
- [ ] UTF-8 support
