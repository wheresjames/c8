#pragma once


template <typename... Args>
    std::string c8_join(const std::string &sep, Args&&... args)
    {
        std::ostringstream oss;
        size_t argc = 0;

        ([&](const auto& arg)
        {   if (argc++)
                oss << sep;
            oss << arg;
        }(std::forward<Args>(args)), ...);

        return oss.str();
    }
