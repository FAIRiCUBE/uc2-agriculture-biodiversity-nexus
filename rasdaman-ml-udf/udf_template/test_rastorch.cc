/*
 * This file is part of rasdaman community.
 *
 * Rasdaman community is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Rasdaman community is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rasdaman community.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2003, 2004, 2005, 2006, 2007, 2008, 2009 Peter Baumann /
 rasdaman GmbH.
 *
 * For more information please see <http://www.rasdaman.org>
 * or contact Peter Baumann via <baumann@rasdaman.com>.
 */
/*************************************************************
 *
 * NAME
 *  test_rastorch.cc - C++ example to execute a rasdaman UDF
 *
 * SYNOPSIS
 *  test_rastorch
 *
 * DESCRIPTION
 *  Calls the gmarray2Tensor UDF function passing a 5x5
 *  r_GMarray. The
 *
 * EXAMPLE
 *  test_rastorch
 *
 ************************************************************/

#include "raslib/minterval.hh"
#include "raslib/odmgtypes.hh"
#include "raslib/error.hh"
#include "raslib/type.hh"
#include "rasodmg/database.hh"
#include "rasodmg/transaction.hh"
#include "rasodmg/set.hh"
#include "rasodmg/ref.hh"
#include "rasodmg/gmarray.hh"
#include "rasodmg/iterator.hh"
#include "rasodmg/oqlquery.hh"

// needed to configure logging
#include "loggingutils.hh"
#include <iostream>

// Implemented extern functions.
extern "C" r_GMarray *predictionTest(r_GMarray *s2_cutout);

INITIALIZE_EASYLOGGINGPP

int main(int ac, char **av)
{
    int returnValue = 0;

    // setup logging configuration
    common::LogConfiguration logConf;
    logConf.configClientLogging();

    char rasmgrName[255];
    int rasmgrPort;
    char baseName[255];
    char userName[255];
    char userPass[255];

    strcpy(rasmgrName, "localhost");
    rasmgrPort = strtoul("7001", NULL, 0);
    strcpy(baseName, "RASBASE");
    strcpy(userName, "rasadmin");
    strcpy(userPass, "rasadmin");

    std::string collName = std::string("sentinel2_2018_flevopolder_10m_7x4bands");
    // std::string collName = std::string("test_s2_tub_v2");

    r_Database database;
    r_Transaction transaction{&database};
    r_Set<r_Ref<r_GMarray>> image_set;
    r_Ref<r_GMarray> image;
    r_Iterator<r_Ref<r_GMarray>> iter;

    try
    {
        database.set_servername(rasmgrName, rasmgrPort);
        database.set_useridentification(userName, userPass);

        std::cout << "Opening database " << baseName
                  << " on " << rasmgrName << "... " << std::flush;

        database.open(baseName);
        std::cout << "OK" << std::endl;

        std::cout << "Starting read-only transaction ... " << std::flush;
        transaction.begin(r_Transaction::read_only);
        std::cout << "OK" << std::endl;

        std::cout << "Creating the query object ..." << std::flush;
        // r_OQL_Query query("select a[250:252,200:202] from $1 as a");
        r_OQL_Query query("select a[250:258,200:208] from $1 as a");
        // r_OQL_Query query("select a from $1 as a");
        // r_OQL_Query query("select a from $1 as a");
        std::cout << "OK, Query string is: " << query.get_query() << std::endl;

        std::cout << "Substituting query parameters ..." << std::flush;
        query << collName.c_str();
        std::cout << "OK, Query string is: " << query.get_query() << std::endl;

        std::cout << "Executing the query ..." << std::flush;
        try
        {
            r_oql_execute(query, image_set, &transaction);
        }
        catch (r_Error &errorObj)
        {
            std::cout << "FAILED" << std::endl
                      << errorObj.what() << std::endl;

            std::cout << "Aborting transaction ... " << std::flush;
            transaction.abort();
            std::cout << "OK" << std::endl;

            std::cout << "Closing database ... " << std::flush;
            database.close();
            std::cout << "OK" << std::endl;
            return -1;
        }
        std::cout << "OK" << std::endl
                  << std::endl;

        std::cout << "Collection" << std::endl;
        std::cout << "  Oid...................: " << image_set.get_oid() << std::endl;
        if (image_set.get_object_name())
        {
            std::cout << "  Type Name.............: " << image_set.get_object_name() << std::endl;
        }
        std::cout << "  Type Structure........: "
                  << (image_set.get_type_structure() ? image_set.get_type_structure() : "<nn>")
                  << std::endl;
        std::cout << "  Type Schema...........: " << std::flush;
        if (image_set.get_type_schema())
        {
            image_set.get_type_schema()->print_status(std::cout);
        }
        else
        {
            std::cout << "<nn>" << std::flush;
        }
        std::cout << std::endl;
        std::cout << "  Number of entries.....: " << image_set.cardinality() << std::endl;
        std::cout << "  Element Type Schema...: " << std::flush;
        if (image_set.get_element_type_schema())
        {
            image_set.get_element_type_schema()->print_status(std::cout);
        }
        else
        {
            std::cout << "<nn>" << std::flush;
        }
        std::cout << std::endl
                  << std::endl;

        iter = image_set.create_iterator();

        int i;
        for (i = 1, iter.reset(); iter.not_done(); iter++, i++)
        {
            std::cout << "Image " << i << std::endl;
            image = *iter;
            image->print_status(std::cout);
            std::cout << std::endl;

            r_GMarray *array = &(*image);
            r_GMarray *result = predictionTest(array);

            std::cout << "Printing from the other side " << std::endl;

            const auto& r_domArray = result->spatial_domain(); // Get the GMarray spatial domain. e.g., [0:250, 0:250]
            size_t r_arraySize = r_domArray.cell_count(); // Get the size of the array

            r_Long* r_array = (r_Long*)result->get_array();    

            std::cout << "Print the values of r_array..." << std::endl;
            for (size_t r_iter = 0; r_iter < r_arraySize; ++r_iter)
            {
                auto temp = *(r_array + r_iter);
                std::cout << "*(r_array" << " + " << r_iter << ") = " << temp << std::endl; 
            }


        }

        std::cout << std::endl;

        std::cout << "Closing database ... " << std::flush;
        database.close();
        std::cout << "OK" << std::endl;
    }
    catch (r_Error &errorObj)
    {
        std::cerr << errorObj.what() << std::endl;
        return -1;
    }

    return returnValue;
} // End int main(int ac, char** av)

/*
 * end of test_rastorch.cc
 */
