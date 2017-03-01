package ua.kh.bars.dao;

import ua.kh.bars.entity.User;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import java.util.Date;


public class UserDAO implements CrudDAO<User, Long> {

    private JdbcTemplate jdbcTemplate;
    private static final String selectAllSQLQuery = "SELECT * FROM users";
    private static final String selectOneSQLQuery = selectAllSQLQuery + " WHERE id = ? LIMIT 1";
    private static final String selectCountSQLQuery = "SELECT COUNT(id) FROM users";
    private static final String deleteOneSQLQuery = "DELETE FROM users WHERE id = ?";
    private static final String saveUserSQLQuery = "REPLACE INTO users " +
        "(id, email, name, tickets, birthday, systemMessage) VALUES (?, ?, ?, ?, ?, ?)";
    private static final String findByNameSQLQuery = selectAllSQLQuery + " WHERE name = ?";
    private static final String findByEmailSQLQuery = selectAllSQLQuery + " WHERE email = ? LIMIT 1";
    
    public UserDAO(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    };

    public User findOne(Long key) {
        return (User)jdbcTemplate.queryForObject( selectOneSQLQuery,
                                                  new Object[]{key},
                                                  new BeanPropertyRowMapper(User.class) );
    };

    public Iterable<User> findAll() {
        return jdbcTemplate.query( selectAllSQLQuery,
                                   new BeanPropertyRowMapper(User.class) );
    };

    public Long count() {
        return jdbcTemplate.queryForObject( selectCountSQLQuery, Long.class );
    };

    public void delete(User user) {
        jdbcTemplate.update(deleteOneSQLQuery, user.getId());
    };

    public void save(User user) {
        jdbcTemplate.update( saveUserSQLQuery, user.getId(), user.getEmail(),
                             user.getName(), user.getTickets(),
                             user.getBirthday(), user.getSystemMessage());
    };

    public boolean exists(Long key) {
        if(findOne(key) != null) {
            return true;
        }
        return false;
    };

    public Iterable<User> findByName(String name) {
        return jdbcTemplate.query( findByNameSQLQuery,
                                   new Object[]{name},
                                   new BeanPropertyRowMapper(User.class) );
    }

    public User findByEmail(String email) {
        return (User)jdbcTemplate.queryForObject( findByEmailSQLQuery,
                                                  new Object[]{email},
                                                  new BeanPropertyRowMapper(User.class) );
    }
}
