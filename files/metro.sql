
CREATE TABLE metro_pp(
    id   SERIAL,
    p1   VARCHAR(128) NOT NULL,      -- 端点 1
    p2   VARCHAR(128) NOT NULL,      -- 端点 2
    dut  INTEGER      NOT NULL,      -- 耗时(秒计)
    dis  INTEGER,                    -- 距离(米计)
    lnos VARCHAR(16)[],              -- 所在线路
    PRIMARY KEY(id)
);

-- 存入地铁信息
INSERT INTO metro_pp(p1,p2,dut)
            VALUES ('南京西路', '陕西南路',180),
                   ('汉中路', '南京西路',147);


-- 转为 json
\COPY (WITH t AS (SELECT * FROM metro_pp) SELECT array_to_json(array_agg(row_to_json(t))) FROM t) TO '/tmp/metro.json';

