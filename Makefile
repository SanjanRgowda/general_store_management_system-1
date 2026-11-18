# =====================================================
# General Store Management System - Makefile
# =====================================================

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iinclude
LIBS = -lsqlite3 -lcrypto -pthread

APP = store

# =====================================================
# BUILD APPLICATION
# =====================================================

all:
	$(CXX) $(CXXFLAGS) \
	main.cpp \
	src/Database.cpp \
	src/Authentication.cpp \
	src/Product.cpp \
	src/Customer.cpp \
	$(LIBS) \
	-o $(APP)

# =====================================================
# GOOGLE TEST BUILDS
# =====================================================

auth_test:
	$(CXX) $(CXXFLAGS) \
	src/test_authentication.cpp \
	src/Authentication.cpp \
	src/Database.cpp \
	-lgtest -lgtest_main \
	$(LIBS) \
	-o test_auth

product_test:
	$(CXX) $(CXXFLAGS) \
	src/test_product.cpp \
	src/Product.cpp \
	src/Database.cpp \
	-lgtest -lgtest_main \
	$(LIBS) \
	-o test_product

customer_test:
	$(CXX) $(CXXFLAGS) \
	src/test_customer.cpp \
	src/Customer.cpp \
	src/Database.cpp \
	-lgtest -lgtest_main \
	$(LIBS) \
	-o test_customer

database_test:
	$(CXX) $(CXXFLAGS) \
	src/test_database.cpp \
	src/Database.cpp \
	-lgtest -lgtest_main \
	$(LIBS) \
	-o test_database

# =====================================================
# GTEST REPORTS (TXT + XML)
# =====================================================

gtest_auth:
	./test_auth \
	--gtest_output=xml:auth_test_report.xml \
	> auth_test_report.txt 2>&1

gtest_product:
	./test_product \
	--gtest_output=xml:product_test_report.xml \
	> product_test_report.txt 2>&1

gtest_customer:
	./test_customer \
	--gtest_output=xml:customer_test_report.xml \
	> customer_test_report.txt 2>&1

gtest_database:
	./test_database \
	--gtest_output=xml:database_test_report.xml \
	> database_test_report.txt 2>&1

gtest_all:
	./test_auth \
	--gtest_output=xml:auth_test_report.xml \
	> auth_test_report.txt 2>&1

	./test_product \
	--gtest_output=xml:product_test_report.xml \
	> product_test_report.txt 2>&1

	./test_customer \
	--gtest_output=xml:customer_test_report.xml \
	> customer_test_report.txt 2>&1

	./test_database \
	--gtest_output=xml:database_test_report.xml \
	> database_test_report.txt 2>&1

# =====================================================
# GCOV COVERAGE
# =====================================================

coverage_auth:
	$(CXX) $(CXXFLAGS) \
	-fprofile-arcs -ftest-coverage \
	src/test_authentication.cpp \
	src/Authentication.cpp \
	src/Database.cpp \
	-lgtest -lgtest_main \
	$(LIBS) \
	-o test_auth
	./test_auth > auth_coverage_execution.txt 2>&1
	gcov test_auth-Authentication.gcno > auth_gcov_report.txt 2>&1

coverage_product:
	$(CXX) $(CXXFLAGS) \
	-fprofile-arcs -ftest-coverage \
	src/test_product.cpp \
	src/Product.cpp \
	src/Database.cpp \
	-lgtest -lgtest_main \
	$(LIBS) \
	-o test_product
	./test_product > product_coverage_execution.txt 2>&1
	gcov test_product-Product.gcno > product_gcov_report.txt 2>&1

coverage_customer:
	$(CXX) $(CXXFLAGS) \
	-fprofile-arcs -ftest-coverage \
	src/test_customer.cpp \
	src/Customer.cpp \
	src/Database.cpp \
	-lgtest -lgtest_main \
	$(LIBS) \
	-o test_customer
	./test_customer > customer_coverage_execution.txt 2>&1
	gcov test_customer-Customer.gcno > customer_gcov_report.txt 2>&1

coverage_database:
	$(CXX) $(CXXFLAGS) \
	-fprofile-arcs -ftest-coverage \
	src/test_database.cpp \
	src/Database.cpp \
	-lgtest -lgtest_main \
	$(LIBS) \
	-o test_database
	./test_database > database_coverage_execution.txt 2>&1
	gcov test_database-Database.gcno > database_gcov_report.txt 2>&1

coverage_all:
	$(MAKE) coverage_auth
	$(MAKE) coverage_product
	$(MAKE) coverage_customer
	$(MAKE) coverage_database

# =====================================================
# VALGRIND REPORTS
# =====================================================

valgrind:
	valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--log-file=valgrind_store_report.txt \
	./$(APP)

valgrind_auth:
	valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--log-file=valgrind_auth_report.txt \
	./test_auth

valgrind_product:
	valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--log-file=valgrind_product_report.txt \
	./test_product

valgrind_customer:
	valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--log-file=valgrind_customer_report.txt \
	./test_customer

valgrind_database:
	valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--log-file=valgrind_database_report.txt \
	./test_database

# =====================================================
# HELGRIND REPORTS
# =====================================================

helgrind:
	valgrind \
	--tool=helgrind \
	--log-file=helgrind_store_report.txt \
	./$(APP)

helgrind_auth:
	valgrind \
	--tool=helgrind \
	--log-file=helgrind_auth_report.txt \
	./test_auth

helgrind_product:
	valgrind \
	--tool=helgrind \
	--log-file=helgrind_product_report.txt \
	./test_product

helgrind_customer:
	valgrind \
	--tool=helgrind \
	--log-file=helgrind_customer_report.txt \
	./test_customer

helgrind_database:
	valgrind \
	--tool=helgrind \
	--log-file=helgrind_database_report.txt \
	./test_database

# =====================================================
# CPPCHECK REPORT
# =====================================================

cppcheck:
	cppcheck \
	--enable=all \
	--inconclusive \
	--std=c++17 \
	-I include \
	--suppress=missingIncludeSystem \
	src/*.cpp main.cpp \
	2> cppcheck_report.txt

# =====================================================
# LCOV HTML COVERAGE REPORT
# =====================================================

html_coverage:
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report

# =====================================================
# COMPLETE PIPELINE
# =====================================================

pipeline:
	$(MAKE) auth_test
	$(MAKE) product_test
	$(MAKE) customer_test
	$(MAKE) database_test
	$(MAKE) gtest_all
	$(MAKE) cppcheck
	$(MAKE) coverage_all

# =====================================================
# CLEAN
# =====================================================

clean:
	rm -f store
	rm -f test_auth
	rm -f test_product
	rm -f test_customer
	rm -f test_database
	rm -f *.gcda *.gcno *.gcov
	rm -f *.txt *.xml
	rm -f coverage.info
	rm -rf coverage_report

# =====================================================
# HELP
# =====================================================

help:
	@echo "make all"
	@echo "make auth_test"
	@echo "make product_test"
	@echo "make customer_test"
	@echo "make database_test"
	@echo "make gtest_auth"
	@echo "make gtest_product"
	@echo "make gtest_customer"
	@echo "make gtest_database"
	@echo "make gtest_all"
	@echo "make coverage_all"
	@echo "make valgrind_auth"
	@echo "make valgrind_product"
	@echo "make valgrind_customer"
	@echo "make valgrind_database"
	@echo "make helgrind_auth"
	@echo "make helgrind_product"
	@echo "make helgrind_customer"
	@echo "make helgrind_database"
	@echo "make cppcheck"
	@echo "make html_coverage"
	@echo "make pipeline"
	@echo "make clean"
