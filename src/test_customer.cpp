#include <gtest/gtest.h>
#include <sstream>

#include "Customer.h"
#include "Database.h"

class CustomerTest : public ::testing::Test
{
protected:
    Database db;
    CustomerManager* customer;

    void SetUp() override
    {
        ASSERT_TRUE(db.connect());
        ASSERT_TRUE(db.initializeTables());

        db.execute("DELETE FROM payments;");
        db.execute("DELETE FROM order_items;");
        db.execute("DELETE FROM orders;");
        db.execute("DELETE FROM products;");
        db.execute("DELETE FROM users;");

        db.execute(
            "INSERT INTO users(username,password_hash,role) "
            "VALUES('customer1','dummy','customer');"
        );

        db.execute(
            "INSERT INTO products(name,quantity,price) "
            "VALUES('Rice',50,45.0);"
        );

        db.execute(
            "INSERT INTO products(name,quantity,price) "
            "VALUES('Milk',20,30.0);"
        );

        customer = new CustomerManager(
            db,
            1,
            "customer1"
        );
    }

    void TearDown() override
    {
        delete customer;
        db.disconnect();
    }
};

// Display Products

TEST_F(CustomerTest, DisplayProducts)
{
    EXPECT_NO_THROW(
        customer->displayProducts()
    );
}

// Search Product Found

TEST_F(CustomerTest, SearchProductFound)
{
    std::istringstream input("Rice\n");
    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->searchProducts()
    );

    std::cin.rdbuf(old);
}

// Search Product Not Found

TEST_F(CustomerTest, SearchProductNotFound)
{
    std::istringstream input("UnknownItem\n");
    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->searchProducts()
    );

    std::cin.rdbuf(old);
}

// Add To Cart Success

TEST_F(CustomerTest, AddToCartSuccess)
{
    std::istringstream input("1\n2\n");
    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->addToCart()
    );

    std::cin.rdbuf(old);
}

// Invalid Product ID

TEST_F(CustomerTest, AddToCartInvalidProductId)
{
    std::istringstream input("-1\n");
    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->addToCart()
    );

    std::cin.rdbuf(old);
}

// Invalid Quantity

TEST_F(CustomerTest, AddToCartInvalidQuantity)
{
    std::istringstream input("1\n-5\n");
    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->addToCart()
    );

    std::cin.rdbuf(old);
}

// Product Not Found

TEST_F(CustomerTest, AddToCartProductNotFound)
{
    std::istringstream input("999\n1\n");
    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->addToCart()
    );

    std::cin.rdbuf(old);
}

// Insufficient Stock

TEST_F(CustomerTest, AddToCartInsufficientStock)
{
    std::istringstream input("1\n500\n");
    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->addToCart()
    );

    std::cin.rdbuf(old);
}

// View Empty Cart

TEST_F(CustomerTest, ViewEmptyCart)
{
    EXPECT_NO_THROW(
        customer->viewCart()
    );
}

// View Cart After Adding Item

TEST_F(CustomerTest, ViewCartWithItems)
{
    {
        std::istringstream input("1\n2\n");
        auto* old = std::cin.rdbuf(input.rdbuf());

        customer->addToCart();

        std::cin.rdbuf(old);
    }

    EXPECT_NO_THROW(
        customer->viewCart()
    );
}

// Empty Order History

TEST_F(CustomerTest, EmptyOrderHistory)
{
    EXPECT_NO_THROW(
        customer->viewOrderHistory()
    );
}

// Existing Order History

TEST_F(CustomerTest, ExistingOrderHistory)
{
    db.execute(
        "INSERT INTO orders(user_id,total_amount,status) "
        "VALUES(1,100,'completed');"
    );

    EXPECT_NO_THROW(
        customer->viewOrderHistory()
    );
}

// Customer Menu Logout

TEST_F(CustomerTest, CustomerMenuLogout)
{
    std::istringstream input("0\n");
    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->customerMenu()
    );

    std::cin.rdbuf(old);
}

// Customer Menu Invalid Option

TEST_F(CustomerTest, CustomerMenuInvalidChoice)
{
    std::istringstream input("99\n0\n");
    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->customerMenu()
    );

    std::cin.rdbuf(old);
}

// Customer Menu View Products

TEST_F(CustomerTest, CustomerMenuViewProducts)
{
    std::istringstream input("1\n0\n");
    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->customerMenu()
    );

    std::cin.rdbuf(old);
}

// Customer Menu Search Product

TEST_F(CustomerTest, CustomerMenuSearchProduct)
{
    std::istringstream input(
        "2\nRice\n0\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        customer->customerMenu()
    );

    std::cin.rdbuf(old);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}