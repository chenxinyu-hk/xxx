#ifndef XXX_TEST_H
#define XXX_TEST_H

#include <stdio.h>
#include <string>
#include <vector>
#include <unordered_map>

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define TEST(test_name) \
    static_assert(sizeof(#test_name) > 1, "TEST() requires a test name"); \
    class CONCAT(test_, test_name) : public TestBase { \
    public: \
        int line() override { return __LINE__; } \
        const char* file() override { return __FILE__; } \
        const char* name() override { return #test_name; } \
        void run() override; \
    }; \
    TestBase* CONCAT(create_test_, test_name)() { return new test_##test_name(); } \
    static TestRegister CONCAT(test_, __COUNTER__)(__FILE__, CONCAT(create_test_, test_name)); \
    void CONCAT(test_, test_name)::run()

#define ASSERT(expr, ...) \
    do { \
        if (!(expr)) { \
            assert_impl(__FILE__, __LINE__, ##__VA_ARGS__); \
            return; \
        } \
    } while (0)

#define DEFER(expr) auto CONCAT(scoped_, __COUNTER__) = make_scope_guard([&]() { expr; })

class TestContext {
public:
    TestContext()
        : failed(false) {}

    ~TestContext() = default;

    void reset() {
        failed = false;
    }

public:
    bool failed;
    const char* file;
    int line;
    char msg[1024];
};

extern TestContext g_ctx;

class TestBase {
public:
    virtual ~TestBase() = default;
    virtual int line() = 0;
    virtual const char* file() = 0;
    virtual const char* name() = 0;
    virtual void run() = 0;
};

using TestFactory = TestBase* (*)();

class TestRegister {
public:
    static std::unordered_map<std::string, std::vector<TestFactory>>& TestManager() {
        static std::unordered_map<std::string, std::vector<TestFactory>> test_manager;
        return test_manager;
    }

    TestRegister(const char* file, TestFactory factory) {
        TestManager()[file].push_back(factory);
    }
};

static inline
void assert_impl(const char* file, int line) {
    g_ctx.failed = true;
    g_ctx.file = file;
    g_ctx.line = line;
    g_ctx.msg[0] = '\0';
}

static inline
void assert_impl(const char* file, int line, const char* msg) {
    g_ctx.failed = true;
    g_ctx.file = file;
    g_ctx.line = line;
    snprintf(g_ctx.msg, sizeof(g_ctx.msg), "%s", msg);
}

template <typename... Args>
static inline
void assert_impl(const char* file, int line, const char* fmt, Args&&... args) {
    g_ctx.failed = true;
    g_ctx.file = file;
    g_ctx.line = line;
    snprintf(g_ctx.msg, sizeof(g_ctx.msg), fmt, std::forward<Args>(args)...);
}

template <typename F>
class ScopeGuard {
public:
    explicit ScopeGuard(F&& f)
        : _func(std::forward<F>(f)) {}
    
    ~ScopeGuard() noexcept {
        _func();
    }
    
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    ScopeGuard(ScopeGuard&&) = default;

private:
    F _func;
};

template <typename F>
ScopeGuard<F> make_scope_guard(F&& f) {
    return ScopeGuard<F>(std::forward<F>(f));
}

#endif
