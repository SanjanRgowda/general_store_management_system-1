#include <gtest/gtest.h>
#include <sstream>

#include "Product.h"
#include "Database.h"

class ProductTest : public ::testing::Test
{
protected:
    Database db;
    ProductManager* product;

    void SetUp() override
    {
        ASSERT_TRUE(db.connect());
        ASSERT_TRUE(db.initializeTables());

        db.execute("DELETE FROM payments;");
        db.execute("DELETE FROM order_items;");
        db.execute("DELETE FROM orders;");
        db.execute("DELETE FROM products;");
        db.execute("DELETE FROM users;");

        product = new ProductManager(db);

        db.execute(
            "INSERT INTO products(name,quantity,price) "
            "VALUES('Rice',100,50.0);"
        );

        db.execute(
            "INSERT INTO products(name,quantity,price) "
            "VALUES('Milk',50,30.0);"
        );
    }

    void TearDown() override
    {
        delete product;
        db.disconnect();
    }
};

// ===== ADD PRODUCT TESTS =====

TEST_F(ProductTest, AddProductSuccess)
{
    std::istringstream input(
        "Sugar\n20\n40\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->addProduct()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, AddProductEmptyName)
{
    std::istringstream input("\n");

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->addProduct()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, AddProductInvalidQuantity)
{
    std::istringstream input(
        "Sugar\n-10\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->addProduct()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, AddProductInvalidPrice)
{
    std::istringstream input(
        "Sugar\n10\n-5\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->addProduct()
    );

    std::cin.rdbuf(old);
}

// ===== DISPLAY PRODUCTS =====

TEST_F(ProductTest, DisplayProducts)
{
    EXPECT_NO_THROW(
        product->displayProducts()
    );
}

TEST_F(ProductTest, DisplayProductsEmptyInventory)
{
    db.execute("DELETE FROM products;");

    EXPECT_NO_THROW(
        product->displayProducts()
    );
}

// ===== SEARCH PRODUCT =====

TEST_F(ProductTest, SearchProductFound)
{
    std::istringstream input(
        "Rice\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->searchProduct()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, SearchProductNotFound)
{
    std::istringstream input(
        "Laptop\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->searchProduct()
    );

    std::cin.rdbuf(old);
}

// ===== UPDATE PRODUCT =====

TEST_F(ProductTest, UpdateProductSuccess)
{
    std::istringstream input(
        "1\nPremium Rice\n200\n60\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->updateProduct()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, UpdateProductInvalidId)
{
    std::istringstream input(
        "-1\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->updateProduct()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, UpdateProductInvalidQuantity)
{
    std::istringstream input(
        "1\nRice\n-5\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->updateProduct()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, UpdateProductInvalidPrice)
{
    std::istringstream input(
        "1\nRice\n20\n-10\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->updateProduct()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, UpdateProductNotFound)
{
    std::istringstream input(
        "999\nRice\n20\n50\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->updateProduct()
    );

    std::cin.rdbuf(old);
}

// ===== DELETE PRODUCT =====

TEST_F(ProductTest, DeleteProductSuccess)
{
    std::istringstream input(
        "1\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->deleteProduct()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, DeleteProductInvalidId)
{
    std::istringstream input(
        "-1\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->deleteProduct()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, DeleteProductNotFound)
{
    std::istringstream input(
        "999\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->deleteProduct()
    );

    std::cin.rdbuf(old);
}

// ===== ADMIN MENU =====

TEST_F(ProductTest, AdminMenuLogout)
{
    std::istringstream input(
        "0\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->adminMenu()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, AdminMenuInvalidChoice)
{
    std::istringstream input(
        "99\n0\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->adminMenu()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, AdminMenuDisplayProducts)
{
    std::istringstream input(
        "2\n0\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->adminMenu()
    );

    std::cin.rdbuf(old);
}

TEST_F(ProductTest, AdminMenuSearchProduct)
{
    std::istringstream input(
        "3\nRice\n0\n"
    );

    auto* old = std::cin.rdbuf(input.rdbuf());

    EXPECT_NO_THROW(
        product->adminMenu()
    );

    std::cin.rdbuf(old);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}