package fc;
import petascope.wcps.result.WcpsResult;
import java.util.List;
import java.util.ArrayList;
import petascope.wcps.metadata.model.*;
import java.math.BigDecimal;
import petascope.exceptions.PetascopeException;

/**
 * UDF implementing the rasql fc.predict_crop_class function.
 */
public class PredictCropClass{

  public WcpsResult handle(List<WcpsResult> arguments){
    //we expect 2 argument: two coverage expressions
    if(arguments.size() != 2){
      throw new RuntimeException("The predictCropClass function expects 2 arguments.");
    }

    String rasql = TEMPLATE.replace("$1", arguments.get(0).getRasql())
                           .replace("$2", arguments.get(1).getRasql());

    return new WcpsResult(createMetadata(arguments.get(0)), rasql);
  }

  private WcpsCoverageMetadata createMetadata(WcpsResult inputCoverage){
    WcpsCoverageMetadata result = inputCoverage.getMetadata();
    List<RangeField> rangeFields = new ArrayList<RangeField>();
    rangeFields.add(new RangeField("short", "crop class", null, "predicted crop class", null, "10^0", null, null, null, null));
    result.setRangeFields(rangeFields);
    return result;
  }

  private final static String TEMPLATE = "fc.predict_crop_class($1, $2)";
}
