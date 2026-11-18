#include <gtest/gtest.h>
#include "Database.h"

class DatabaseTest : public ::testing::Test
{
protected:
    Database db;

  void SetUp() override
{
    ASSERT_TRUE(db.connect());
    ASSERT_TRUE(db.initializeTables());

    db.execute("DELETE FROM payments;");
    db.execute("DELETE FROM order_items;");
    db.execute("DELETE FROM orders;");
    db.execute("DELETE FROM products;");
    db.execute("DELETE FROM users;");

    // Reset AUTOINCREMENT counters
    db.execute("DELETE FROM sqlite_sequence;");
}
    void TearDown() override
    {
        db.disconnect();
    }
};

// Constructor + Connect

TEST_F(DatabaseTest, ConnectDatabase)
{
    EXPECT_NE(db.getConnection(), nullptr);
}

// Execute Valid SQL

TEST_F(DatabaseTest, ExecuteValidSQL)
{
    EXPECT_TRUE(
        db.execute(
            "CREATE TABLE IF NOT EXISTS test_table("
            "id INTEGER PRIMARY KEY);"
        )
    );
}

// Execute Invalid SQL

TEST_F(DatabaseTest, ExecuteInvalidSQL)
{
    EXPECT_FALSE(
        db.execute(
            "INVALID SQL STATEMENT"
        )
    );
}

// Initialize Tables

TEST_F(DatabaseTest, InitializeTables)
{
    EXPECT_TRUE(
        db.initializeTables()
    );
}

// Initialize Tables Again

TEST_F(DatabaseTest, InitializeTablesTwice)
{
    EXPECT_TRUE(
        db.initializeTables()
    );

    EXPECT_TRUE(
        db.initializeTables()
    );
}

// Get Connection

TEST_F(DatabaseTest, GetConnection)
{
    sqlite3* connection =
        db.getConnection();

    EXPECT_NE(connection, nullptr);
}

// Create User Table Entry

TEST_F(DatabaseTest, InsertIntoUsers)
{
    ASSERT_TRUE(
        db.initializeTables()
    );

    EXPECT_TRUE(
        db.execute(
            "INSERT INTO users "
            "(username,password_hash,role) "
            "VALUES "
            "('testuser','hash123','customer');"
        )
    );
}

// Duplicate User

TEST_F(DatabaseTest, DuplicateUsernameConstraint)
{
    ASSERT_TRUE(
        db.initializeTables()
    );

    EXPECT_TRUE(
        db.execute(
            "INSERT INTO users "
            "(username,password_hash,role) "
            "VALUES "
            "('duplicate','hash','customer');"
        )
    );

    EXPECT_FALSE(
        db.execute(
            "INSERT INTO users "
            "(username,password_hash,role) "
            "VALUES "
            "('duplicate','hash','customer');"
        )
    );
}

// Insert Product

TEST_F(DatabaseTest, InsertProduct)
{
    ASSERT_TRUE(
        db.initializeTables()
    );

    EXPECT_TRUE(
        db.execute(
            "INSERT INTO products "
            "(name,quantity,price) "
            "VALUES "
            "('Rice',100,50.00);"
        )
    );
}

// Negative Quantity Check

TEST_F(DatabaseTest, ProductNegativeQuantity)
{
    ASSERT_TRUE(
        db.initializeTables()
    );

    EXPECT_FALSE(
        db.execute(
            "INSERT INTO products "
            "(name,quantity,price) "
            "VALUES "
            "('Rice',-1,50.00);"
        )
    );
}

// Negative Price Check

TEST_F(DatabaseTest, ProductNegativePrice)
{
    ASSERT_TRUE(
        db.initializeTables()
    );

    EXPECT_FALSE(
        db.execute(
            "INSERT INTO products "
            "(name,quantity,price) "
            "VALUES "
            "('Rice',10,-50.00);"
        )
    );
}

// Role Constraint

TEST_F(DatabaseTest, InvalidRole)
{
    ASSERT_TRUE(
        db.initializeTables()
    );

    EXPECT_FALSE(
        db.execute(
            "INSERT INTO users "
            "(username,password_hash,role) "
            "VALUES "
            "('user1','hash','invalidrole');"
        )
    );
}

// Foreign Key Validation

TEST_F(DatabaseTest, InvalidOrderForeignKey)
{
    ASSERT_TRUE(
        db.initializeTables()
    );

    EXPECT_FALSE(
        db.execute(
            "INSERT INTO orders "
            "(user_id,total_amount) "
            "VALUES "
            "(9999,100);"
        )
    );
}

// Valid Order Foreign Key

TEST_F(DatabaseTest, ValidOrderInsert)
{
    ASSERT_TRUE(
        db.initializeTables()
    );

    EXPECT_TRUE(
        db.execute(
            "INSERT INTO users "
            "(username,password_hash,role) "
            "VALUES "
            "('john','hash','customer');"
        )
    );

    EXPECT_TRUE(
        db.execute(
            "INSERT INTO orders "
            "(user_id,total_amount) "
            "VALUES "
            "(1,200);"
        )
    );
}

// Disconnect

TEST_F(DatabaseTest, DisconnectDatabase)
{
    db.disconnect();

    EXPECT_EQ(
        db.getConnection(),
        nullptr
    );
}

// Multiple Disconnect Calls

TEST_F(DatabaseTest, MultipleDisconnects)
{
    db.disconnect();
    db.disconnect();

    EXPECT_EQ(
        db.getConnection(),
        nullptr
    );
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}