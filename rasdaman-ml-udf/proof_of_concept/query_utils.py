"""
Basic class for Rasdaman queries.
"""

__author__ = "Otoniel Campos"
__date__ = "2023-04-17"

# import modules
import io  
import time  
import datetime as dt  
import requests
from urllib.error import HTTPError
import numpy as np

class Rasdaman_Query(object):
    
    cls_name = "Rasdaman_Query"
    
    def __init__(self, service_endpoint="http://fairicube.rasdaman.com:8080/rasdaman/ows#/services"):
        
        self.service_endpoint = service_endpoint
        _ = self.check_endpoint()
        self.query_history = {}
        self.nquery = 0
        
    def check_endpoint(self, wait_time: int = 10):
        """
        Checks if service endpoint can be reached.
        :param wait_time: Maximum wait time to get response from service endpoint.
        :return bool: True when connection was successful
        """
        method = Rasdaman_Query.check_endpoint.__name__
        
        try:
            _ = requests.get(self.service_endpoint, timeout=wait_time)
            print("%{0}: Selected service endpoint '{1}' reached successfully.".format(method, self.service_endpoint))
        except (requests.ConnectionError, requests.Timeout) as exception:
            print("%{0}: Service endpoint '{1}' could not be reached. Please check URL as well as internet connection."
                  .format(method, self.service_endpoint))
            
        return True
    
    def get_query(self, query_str: str, credentials):
        """
        Function returns a query as a netcdf byte-encoded response.
        :param query_str: WCPS query.
        :turns xarray.Dataset
        """
        method = Rasdaman_Query.get_query.__name__
        
        assert isinstance(query_str, str), \
               "%{0}: Query must be a string-object, but is of type '{1}'.".format(method, type(query_str))

        print("The query is...\n {0}".format(query_str))

        try:
            time0 = time.time()
            print("%{0}: Start query...".format(method))
            query_response = requests.post(self.service_endpoint, data={'query': query_str}, auth=credentials)   
            query_response.raise_for_status()
            # Convert bytes to file-like object
            ds = io.BytesIO(query_response.content)
            # track time and populate query history
            time_tot = time.time() - time0
            query_dict = {"query string": query_str, "loading time": time_tot, 
                          "size data (MB)": len(ds.getbuffer()) / (1024*1024)}
            self.query_history["query_{0:d}".format(self.nquery)] = query_dict 
            self.nquery += 1
            print("%{0}: Data query took {1:5.2f} seconds.".format(method, time_tot))
        except HTTPError as err:
            print("%{0}: Query '{1}' failed. See raised HTTPError-message.".format(method, query_str))
            raise err
        except Exception as err:
            print("%{0}: Unknown error occurred with query '{1}'.".format(method, query_str))
            raise err
                
        return ds

