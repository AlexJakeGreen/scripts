package ua.kh.bars.dao;

import ua.kh.bars.entity.Event;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import java.sql.SQLException;
import java.sql.ResultSet;
import java.util.Date;


public class CounterDAO {

    private JdbcTemplate jdbcTemplate;
    public CounterDAO(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }

    public void saveCounter(String counterName, Event event, int count) {
        jdbcTemplate.update("REPLACE INTO counters(id, name, type, key, value) " +
                            "VALUES ((SELECT id FROM counters WHERE name = ? AND type = ? and key = ?), ?, ?, ?, ?)",
                            new Object[]{counterName,
                                         event.getClass().getSimpleName(),
                                         event.getId(),
                                         counterName,
                                         event.getClass().getSimpleName(),
                                         event.getId(),
                                         count});
    }

    public int getCount(String counterName, Event entity) {
        Integer count =
            jdbcTemplate.queryForInt("SELECT COALESCE((SELECT value FROM counters WHERE name = ? AND type = ? AND key = ? LIMIT 1), 0)",
                                     counterName,
                                     entity.getClass().getSimpleName(), entity.getId());
        if (count == null) {
            return 0;
        }
        return count;
    }

    public void printCounters() {
        jdbcTemplate.query("SELECT * FROM counters",
                           new RowMapper(){
                               public Object mapRow(ResultSet rs, int rowNum) throws SQLException {
                                   System.out.println(rs.getLong("id") + "|" +
                                                      rs.getString("name") + "|" +
                                                      rs.getString("type") + "|" +
                                                      rs.getInt("key") + "|" +
                                                      rs.getString("value"));
                                   return null;
                               }
                           });
    }
    
}
