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
 *  test_predict_crop_class.cc - C++ example to execute a rasdaman UDF
 *
 * SYNOPSIS
 *  test_predict_crop_class
 *
 * DESCRIPTION
 *  Calls the gmarray2Tensor UDF function passing a 5x5
 *  r_GMarray. The
 *
 * EXAMPLE
 *  test_predict_crop_class
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
#include <string>

// extern functions (UDFs)
// extern "C" r_GMarray* predictCropClass(r_GMarray *inputImg);
extern "C" r_GMarray* predictCropClass(r_GMarray *inputImg, r_GMarray *maxesPerBand);

INITIALIZE_EASYLOGGINGPP

char rasmgrName[255] = "localhost";
int rasmgrPort = 7001;
char baseName[255] = "RASBASE";
char userName[255] = "rasadmin";
char userPass[255] = "rasadmin";

int main(int ac, char **av)
{
    // setup logging configuration
    common::LogConfiguration logConf;
    logConf.configClientLogging();

    // if (ac != 6)
    // {
    //     std::cout << "Usage: query HOST PORT DATABASE USER PASSWORD" << std::endl;
    //     return -1;
    // }

    // strcpy(rasmgrName, av[1]);
    // rasmgrPort = strtoul(av[2], NULL, 0);
    // strcpy(baseName, av[3]);
    // strcpy(userName, av[4]);
    // strcpy(userPass, av[5]);

    std::string collName = std::string("sentinel2_2018_flevopolder_10m_7x4bands");
    r_Minterval select_domain = r_Minterval("[0:8,0:8]");
    r_Minterval where_domain = r_Minterval("[8:9,8:9]");
    r_ULong threshold_value = 0;

    r_OQL_Query query("select a[0:8,0:8] from $1 as a");

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
        // r_OQL_Query query("select a$1 from $2 as a where some_cells( a$3 >= $4 )");
        std::cout << "OK, Query string is: " << query.get_query() << std::endl;

        std::cout << "Substituting query parameters ..." << std::flush;
        query << select_domain << collName.c_str();
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
        }
        std::cout << std::endl;

        std::cout << "Committing transaction ... " << std::flush;
        transaction.commit();
        std::cout << "OK" << std::endl;

        std::cout << "Closing database ... " << std::flush;
        database.close();
        std::cout << "OK" << std::endl;
    }
    catch (r_Error &errorObj)
    {
        std::cerr << errorObj.what() << std::endl;
        return -1;
    }

    return 0;
}
