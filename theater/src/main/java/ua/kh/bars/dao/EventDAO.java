package ua.kh.bars.dao;

import java.util.Collection;
import java.util.Date;
import org.springframework.jdbc.core.JdbcTemplate;
import ua.kh.bars.entity.Auditorium;
import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.EventRating;

public class EventDAO implements CrudDAO<Event, Date> {

    private JdbcTemplate jdbcTemplate;

    private static final String selectAllSQLQuery =
        "SELECT e.id AS eventId, " +
        "       e.date AS eventDate, " +
        "       e.name AS eventName, " +
        "       e.price AS eventPrice, " +
        "       e.rating AS eventRating, " +
        "       a.id AS auditoriumId, " +
        "       a.name AS auditoriumName, " +
        "       a.seats AS auditoriumSeats, " +
        "       a.vipSeats AS auditoriumVipSeats " +
        "FROM events e " +
        "LEFT JOIN auditoriums a ON e.auditorium = a.id";
    private static final String findByNameSQLQuery = selectAllSQLQuery + " WHERE e.name = ?";
    private static final String findByDateSQLQuery = selectAllSQLQuery + " WHERE e.date = ? LIMIT 1";
    private static final String selectCountSQLQuery = "SELECT COUNT(id) FROM events";
    private static final String deleteSQLQuery = "DELETE FROM events WHERE id = ?";
    private static final String updateSQLQuery = "REPLACE INTO events " +
        "(id, date, name, price, rating, auditorium) VALUES (?, ?, ?, ?, ?, ?)";
    
    public EventDAO(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    };

    public Event findOne(Date key) {
        return (Event)jdbcTemplate.queryForObject( findByDateSQLQuery,
                                                   new Object[]{key},
                                                   new EventRowMapper());
    };

    public Collection<Event> findAll() {
        return jdbcTemplate.query( selectAllSQLQuery,
                                   new EventRowMapper());
    };

    public Long count() {
        return jdbcTemplate.queryForObject(selectCountSQLQuery, Long.class);
    };

    public void delete(Event event) {
        jdbcTemplate.update(deleteSQLQuery, event.getId());
    };

    public void save(Event event) {
        Long auditoriumId = (event.getAuditorium() == null) ? null : event.getAuditorium().getId();
        jdbcTemplate.update(updateSQLQuery, event.getId(), event.getDate(), event.getName(),
                            event.getPrice(), event.getRating().ordinal(), auditoriumId);
    };

    public boolean exists(Date key) {
        return findOne(key) != null;
    };

    public Iterable<Event> findByName(String name) {
        return jdbcTemplate.query( findByNameSQLQuery,
                                   new Object[]{name},
                                   new EventRowMapper());
    }
}
