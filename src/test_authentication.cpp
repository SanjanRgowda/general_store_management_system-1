#include <gtest/gtest.h>
#include <sstream>
#include "Authentication.h"
#include "Database.h"

class AuthenticationTest : public ::testing::Test
{
protected:
    Database db;
    Authentication* auth;

    void SetUp() override
    {
        ASSERT_TRUE(db.connect());
        ASSERT_TRUE(db.initializeTables());

        db.execute("DELETE FROM payments;");
        db.execute("DELETE FROM order_items;");
        db.execute("DELETE FROM orders;");
        db.execute("DELETE FROM users;");

        auth = new Authentication(db);
    }

    void TearDown() override
    {
        delete auth;
        db.disconnect();
    }
};

// ==========================
// createInitialAdmin()
// ==========================

TEST_F(AuthenticationTest, CreateInitialAdminSuccess)
{
    std::istringstream input(
        "admin\n"
        "Admin12345\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_TRUE(
        auth->createInitialAdmin()
    );

    std::cin.rdbuf(old);
}

TEST_F(AuthenticationTest, CreateInitialAdminAlreadyExists)
{
    std::istringstream input(
        "admin\n"
        "Admin12345\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_TRUE(
        auth->createInitialAdmin()
    );

    std::cin.rdbuf(old);

    EXPECT_TRUE(
        auth->createInitialAdmin()
    );
}

// ==========================
// registerCustomer()
// ==========================

TEST_F(AuthenticationTest, RegisterCustomerSuccess)
{
    std::istringstream adminInput(
        "admin\n"
        "Admin12345\n"
    );

    auto* old = std::cin.rdbuf(adminInput.rdbuf());

    auth->createInitialAdmin();

    std::cin.rdbuf(old);

    std::istringstream customerInput(
        "customer1\n"
        "Password123\n"
    );

    old = std::cin.rdbuf(customerInput.rdbuf());

    EXPECT_TRUE(
        auth->registerCustomer()
    );

    std::cin.rdbuf(old);
}

TEST_F(AuthenticationTest, RegisterCustomerDuplicate)
{
    std::istringstream adminInput(
        "admin\n"
        "Admin12345\n"
    );

    auto* old = std::cin.rdbuf(adminInput.rdbuf());

    auth->createInitialAdmin();

    std::cin.rdbuf(old);

    std::istringstream firstUser(
        "customer1\n"
        "Password123\n"
    );

    old = std::cin.rdbuf(firstUser.rdbuf());

    EXPECT_TRUE(
        auth->registerCustomer()
    );

    std::cin.rdbuf(old);

    std::istringstream duplicateUser(
        "customer1\n"
        "Password123\n"
    );

    old = std::cin.rdbuf(duplicateUser.rdbuf());

    EXPECT_FALSE(
        auth->registerCustomer()
    );

    std::cin.rdbuf(old);
}

// ==========================
// login()
// ==========================

TEST_F(AuthenticationTest, LoginSuccess)
{
    std::istringstream adminInput(
        "admin\n"
        "Admin12345\n"
    );

    auto* old = std::cin.rdbuf(adminInput.rdbuf());

    EXPECT_TRUE(
        auth->createInitialAdmin()
    );

    std::cin.rdbuf(old);

    UserSession session{};

    std::istringstream loginInput(
        "admin\n"
        "Admin12345\n"
    );

    old = std::cin.rdbuf(loginInput.rdbuf());

    EXPECT_TRUE(
        auth->login(session)
    );

    std::cin.rdbuf(old);

    EXPECT_TRUE(session.loggedIn);
}

TEST_F(AuthenticationTest, LoginWrongPassword)
{
    std::istringstream adminInput(
        "admin\n"
        "Admin12345\n"
    );

    auto* old = std::cin.rdbuf(adminInput.rdbuf());

    auth->createInitialAdmin();

    std::cin.rdbuf(old);

    UserSession session{};

    std::istringstream loginInput(
        "admin\n"
        "WrongPass\n"
    );

    old = std::cin.rdbuf(loginInput.rdbuf());

    EXPECT_FALSE(
        auth->login(session)
    );

    std::cin.rdbuf(old);
}

TEST_F(AuthenticationTest, LoginUnknownUser)
{
    UserSession session{};

    std::istringstream input(
        "unknown\n"
        "Password123\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_FALSE(
        auth->login(session)
    );

    std::cin.rdbuf(old);
}

// ==========================
// Main
// ==========================

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}