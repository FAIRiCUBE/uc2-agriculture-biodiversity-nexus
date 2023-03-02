# FAIRiCUBE Agriculture - Biodiversity Nexus Use Case

## About this Use Case

This use case focuses on biodiversity as one of the European Green Deal (EGD) priority actions, while considering the agricultural landscape as focus environment for the investigation of impact of activities at farm field level on the biodiversity. To describe a basic conceptual design of biodiversity assessment within this study we use the Dutch Biodiversity Monitor (DBM), that measures the effect on biodiversity resulting from impact that farming has on the physical conditions on the environment expressed by Key Performance Indicators (KPIs). 

The main goal in this use case is to investigate how a datacube based data infrastructure can improve the access to information related to biodiversity for researchers involved in linking human activities in agricultural areas to changes in the physical conditions of the region (e.g., soil, groundwater, emissions etc.) and the biodiversity. Furthermore, the analysis tools utilized to extract explainable links between different interventions at farm level, specific biodiversity indicators and further derived measures regarding biodiversity will be provided within FAIRiCUBE for reuse both in different locations as well as pertaining to related questions. 


## Research Questions

 * Can the integration and ML-based analysis of currently available biodiversity, agriculture, environmental, and remote sensing data provide comprehensive, verifiable, and actionable insights for different regions?  

 * Can datacube functionality and ML help in finding patterns between effects of farm level measures, indicators of physical conditions and direct measures of biodiversity?  

 * Can the insights obtained in the study region be extended to other regions, learned patterns reused by applying transfer learning?  


## Repository Organization

The repository for this use case contains directories for each of its major objectives. They are as follows:

 1. rasdaman-ml-udf: Proof of concept for adding deep learning model inference to the rasdaman server.
 2. species-occurrences: How to create gridded data cubes from species occurrences data (sparse field (presence) observations).
 3. species-gap-filling: How to synthesize plausible additional species occurrence data using deep learning models (e.g. transformers and generative models).
 4. biodiversity-index: The calculation of a biodiversity index (at scale) based on the species occurrences data.
 5. causal-ml: Causal modelling to find explainable relations between farm management and biodiversity.

---

### Directory layout

The following generic directory layout is used in the subdirectories for the objectives. Additional folders are added when needed.


    ├── LICENSE
    ├── Makefile           <- Makefile with commands like `make data` or `make train`
    ├── README.md          <- The top-level README for developers using this project.
    ├── data
    │   ├── external       <- Data from third party sources.
    │   ├── interim        <- Intermediate data that has been transformed.
    │   ├── processed      <- The final, canonical data sets for modeling.
    │   └── raw            <- The original, immutable data dump.
    │
    ├── docs               <- A default Sphinx project; see sphinx-doc.org for details
    │
    ├── models             <- Trained and serialized models, model predictions, or model summaries
    │
    ├── notebooks          <- Jupyter notebooks. Naming convention is a number (for ordering),
    │                         the creator's initials, and a short `-` delimited description, e.g.
    │                         `1.0-jqp-initial-data-exploration`.
    │
    ├── references         <- Literature, data dictionaries, manuals, and all other explanatory materials.
    │
    ├── reports            <- Generated results as HTML, PPT, PDF, LaTeX, etc.
    │   └── figures        <- Generated graphics and figures to be used in reporting
    │
    ├── requirements.txt   <- The requirements file for reproducing the analysis environment, e.g.
    │                         generated with `pip freeze > requirements.txt`
    │
    ├── setup.py           <- makes project pip installable (pip install -e .) so src can be imported
    ├── src                <- Source code for use in this project.
    │   ├── __init__.py    <- Makes src a Python module
    │   │
    │   ├── data           <- Scripts to download or generate data
    │   │   └── make_dataset.py
    │   │
    │   ├── features       <- Scripts to turn raw data into features for modeling
    │   │   └── build_features.py
    │   │
    │   ├── models         <- Scripts to train models and then use trained models to make
    │   │   │                 predictions
    │   │   ├── predict_model.py
    │   │   └── train_model.py
    │   │
    │   └── visualization  <- Scripts to create exploratory and results oriented visualizations
    │       └── visualize.py
    │
    └── tox.ini            <- tox file with settings for running tox; see tox.readthedocs.io


--------

<p><small>Project based on the <a target="_blank" href="https://drivendata.github.io/cookiecutter-data-science/">cookiecutter data science project template</a>. #cookiecutterdatascience</small></p>

