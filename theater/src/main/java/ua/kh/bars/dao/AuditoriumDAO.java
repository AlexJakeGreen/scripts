package ua.kh.bars.dao;

import ua.kh.bars.entity.Auditorium;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.BeanPropertyRowMapper;

public class AuditoriumDAO implements CrudDAO<Auditorium, Long> {

    private JdbcTemplate jdbcTemplate;
    private static final String findAllSQLQuery = "SELECT * FROM auditoriums";
    private static final String selectOneSQLQuery = findAllSQLQuery + " WHERE id = ?";
    private static final String findByNameSQLQuery = findAllSQLQuery + " WHERE name LIKE ?";
    private static final String selectCountSQLQuery = "SELECT COUNT(id) FROM auditoriums";
    private static final String deleteOneSQLQuery = "DELETE FROM auditoriums WHERE id = ?";
    private static final String saveSQLQuery = "REPLACE INTO users (id, name, seats, vipSeats)  VALUES (?, ?, ?, ?)";

    public AuditoriumDAO() {
    };

    public AuditoriumDAO(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }

    public Auditorium findOne(Long key) {
        return (Auditorium)jdbcTemplate.queryForObject( selectOneSQLQuery,
                                            new Object[]{key},
                                            new BeanPropertyRowMapper(Auditorium.class) );
    };

    public Iterable<Auditorium> findAll() {
        return jdbcTemplate.query( findAllSQLQuery,
                                   new BeanPropertyRowMapper(Auditorium.class) );
    };

    public Long count() {
        return jdbcTemplate.queryForObject( selectCountSQLQuery, Long.class );
    };

    public void delete(Auditorium auditorium) {
        jdbcTemplate.update(deleteOneSQLQuery, auditorium.getId());
    };

    public void save(Auditorium auditorium) {
        jdbcTemplate.update( saveSQLQuery, auditorium.getId(), auditorium.getName(),
                             auditorium.getSeats(), auditorium.getVipSeats());
    };

    public boolean exists(Long key) {
        return findOne(key) != null;
    };

    public Iterable<Auditorium> findByName(String name) {
        return jdbcTemplate.query( findByNameSQLQuery,
                                   new BeanPropertyRowMapper(Auditorium.class) );
    }
}
