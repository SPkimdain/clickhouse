#include <Parsers/tests/gtest_common.h>

#include <Parsers/Kusto/ParserKQLQuery.h>

INSTANTIATE_TEST_SUITE_P(ParserKQLQuery_MakeSeries, ParserKQLTest,
    ::testing::Combine(
        ::testing::Values(std::make_shared<DB::ParserKQLQuery>()),
        ::testing::ValuesIn(std::initializer_list<ParserTestCase>{
        {
            "T |  make-series PriceAvg = avg(Price) default=0 on Purchase from datetime(2016-09-10)  to datetime(2016-09-13) step 1d by Supplier, Fruit",
            "SELECT\n    Supplier,\n    Fruit,\n    zipped.1 AS Purchase,\n    zipped.2 AS PriceAvg\nFROM\n(\n    SELECT\n        toUInt64(min(Purchase_ali)) AS low,\n        toUInt64(max(Purchase_ali)) + 86400 AS high,\n        arraySort(arrayZip(Purchase, PriceAvg)) AS zipped,\n        Supplier,\n        Fruit,\n        arrayConcat(groupArray(PriceAvg_ali) AS ga, arrayMap(x -> 0, range(0, toUInt32(if((length(range(toUInt64(toDateTime64('2016-09-10', 9, 'UTC')), toUInt64(toDateTime64('2016-09-13', 9, 'UTC')), toUInt64(86400))) - length(ga)) < 0, 0, length(range(toUInt64(toDateTime64('2016-09-10', 9, 'UTC')), toUInt64(toDateTime64('2016-09-13', 9, 'UTC')), toUInt64(86400))) - length(ga))), 1))) AS PriceAvg,\n        arrayDistinct(arrayConcat(groupArray(toDateTime64(Purchase_ali - 0, 9, 'UTC')), arrayMap(x -> toDateTime64(x - 0, 9, 'UTC'), range(toUInt64(toDateTime64('2016-09-10', 9, 'UTC')), toUInt64(toDateTime64('2016-09-13', 9, 'UTC')), toUInt64(86400))))) AS Purchase\n    FROM\n    (\n        SELECT\n            Supplier,\n            Fruit,\n            avg(Price) AS PriceAvg_ali,\n            toFloat64(toDateTime64('2016-09-10', 9, 'UTC')) + (toInt64((toFloat64(toDateTime64(Purchase, 9, 'UTC')) - toFloat64(toDateTime64('2016-09-10', 9, 'UTC'))) / 86400) * 86400) AS Purchase_ali\n        FROM T\n        WHERE (toInt64(toFloat64(toDateTime64(Purchase, 9, 'UTC'))) >= toUInt64(toDateTime64('2016-09-10', 9, 'UTC'))) AND (toInt64(toFloat64(toDateTime64(Purchase, 9, 'UTC'))) < toUInt64(toDateTime64('2016-09-13', 9, 'UTC')))\n        GROUP BY\n            Supplier,\n            Fruit,\n            Purchase_ali\n        ORDER BY Purchase_ali ASC\n    )\n    GROUP BY\n        Supplier,\n        Fruit\n)"
        },
        {
            "T2 | make-series PriceAvg=avg(Price) default=0 on Purchase from 10 to  15 step  1.0  by Supplier, Fruit",
            "SELECT\n    Supplier,\n    Fruit,\n    zipped.1 AS Purchase,\n    zipped.2 AS PriceAvg\nFROM\n(\n    SELECT\n        toUInt64(min(Purchase_ali)) AS low,\n        toUInt64(max(Purchase_ali)) + 1 AS high,\n        arraySort(arrayZip(Purchase, PriceAvg)) AS zipped,\n        Supplier,\n        Fruit,\n        arrayConcat(groupArray(PriceAvg_ali) AS ga, arrayMap(x -> 0, range(0, toUInt32(if((length(range(toUInt64(10), toUInt64(15), toUInt64(1))) - length(ga)) < 0, 0, length(range(toUInt64(10), toUInt64(15), toUInt64(1))) - length(ga))), 1))) AS PriceAvg,\n        arrayDistinct(arrayConcat(groupArray(Purchase_ali), arrayMap(x -> toFloat64(x), range(toUInt64(10), toUInt64(15), toUInt64(1))))) AS Purchase\n    FROM\n    (\n        SELECT\n            Supplier,\n            Fruit,\n            avg(Price) AS PriceAvg_ali,\n            toFloat64(10) + (toInt64((toFloat64(Purchase) - toFloat64(10)) / 1) * 1) AS Purchase_ali\n        FROM T2\n        WHERE (toInt64(toFloat64(Purchase)) >= toUInt64(10)) AND (toInt64(toFloat64(Purchase)) < toUInt64(15))\n        GROUP BY\n            Supplier,\n            Fruit,\n            Purchase_ali\n        ORDER BY Purchase_ali ASC\n    )\n    GROUP BY\n        Supplier,\n        Fruit\n)"
        },
        {
            "T |  make-series PriceAvg = avg(Price) default=0 on Purchase step 1d by Supplier, Fruit",
            "SELECT\n    Supplier,\n    Fruit,\n    zipped.1 AS Purchase,\n    zipped.2 AS PriceAvg\nFROM\n(\n    SELECT\n        toUInt64(min(Purchase_ali)) AS low,\n        toUInt64(max(Purchase_ali)) + 86400 AS high,\n        arraySort(arrayZip(Purchase, PriceAvg)) AS zipped,\n        Supplier,\n        Fruit,\n        arrayConcat(groupArray(PriceAvg_ali) AS ga, arrayMap(x -> 0, range(0, toUInt32(if((length(range(low, high, toUInt64(86400))) - length(ga)) < 0, 0, length(range(low, high, toUInt64(86400))) - length(ga))), 1))) AS PriceAvg,\n        arrayDistinct(arrayConcat(groupArray(toDateTime64(Purchase_ali - 62135596800, 9, 'UTC')), arrayMap(x -> toDateTime64(x - 62135596800, 9, 'UTC'), range(low, high, toUInt64(86400))))) AS Purchase\n    FROM\n    (\n        SELECT\n            Supplier,\n            Fruit,\n            avg(Price) AS PriceAvg_ali,\n            toFloat64(toInt64((toFloat64(toDateTime64(Purchase, 9, 'UTC')) + 62135596800) / 86400) * 86400) AS Purchase_ali\n        FROM T\n        GROUP BY\n            Supplier,\n            Fruit,\n            Purchase_ali\n        ORDER BY Purchase_ali ASC\n    )\n    GROUP BY\n        Supplier,\n        Fruit\n)"
        },
        {
            "T2 | make-series PriceAvg=avg(Price) default=0 on Purchase step  1.0  by Supplier, Fruit",
            "SELECT\n    Supplier,\n    Fruit,\n    zipped.1 AS Purchase,\n    zipped.2 AS PriceAvg\nFROM\n(\n    SELECT\n        toUInt64(min(Purchase_ali)) AS low,\n        toUInt64(max(Purchase_ali)) + 1 AS high,\n        arraySort(arrayZip(Purchase, PriceAvg)) AS zipped,\n        Supplier,\n        Fruit,\n        arrayConcat(groupArray(PriceAvg_ali) AS ga, arrayMap(x -> 0, range(0, toUInt32(if((length(range(low, high, toUInt64(1))) - length(ga)) < 0, 0, length(range(low, high, toUInt64(1))) - length(ga))), 1))) AS PriceAvg,\n        arrayDistinct(arrayConcat(groupArray(Purchase_ali), arrayMap(x -> toFloat64(x), range(low, high, toUInt64(1))))) AS Purchase\n    FROM\n    (\n        SELECT\n            Supplier,\n            Fruit,\n            avg(Price) AS PriceAvg_ali,\n            toFloat64(toInt64((toFloat64(Purchase) + 0) / 1) * 1) AS Purchase_ali\n        FROM T2\n        GROUP BY\n            Supplier,\n            Fruit,\n            Purchase_ali\n        ORDER BY Purchase_ali ASC\n    )\n    GROUP BY\n        Supplier,\n        Fruit\n)"
        }
})));