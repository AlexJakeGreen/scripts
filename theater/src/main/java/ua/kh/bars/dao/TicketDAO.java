package ua.kh.bars.dao;

import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.Ticket;
import org.springframework.jdbc.core.JdbcTemplate;
import java.util.Date;

public class TicketDAO implements CrudDAO<Ticket, Long> {

    private JdbcTemplate jdbcTemplate;
    private static final String findAllSQLQuery =
        "SELECT t.id AS ticketId, " +
        "       t.price AS ticketPrice, " +
        "       t.date AS ticketDate, " +
        "       t.seat AS ticketSeat, " +
        "       u.id AS userId, " +
        "       u.name AS userName, " +
        "       u.email AS userEmail, " +
        "       u.tickets AS userTickets, " +
        "       u.birthday AS userBirthday, " +
        "       u.systemMessage AS userSystemMessage, " +
        "       e.id AS eventId, " +
        "       e.date AS eventDate, " +
        "       e.name AS eventName, " +
        "       e.price AS eventPrice, " +
        "       e.rating AS eventRating, " +
        "       a.id AS auditoriumId, " +
        "       a.name AS auditoriumName, " +
        "       a.seats AS auditoriumSeats, " +
        "       a.vipSeats AS auditoriumVipSeats " +
        "FROM tickets t " +
        "LEFT JOIN users u ON t.user = u.id " +
        "LEFT JOIN events e ON t.event = e.id " +
        "LEFT JOIN auditoriums a ON e.auditorium = a.id";
    private static final String findOneSQLQuery = findAllSQLQuery + "WHERE t.id = ?";
    private static final String searchByEventDateSQLQuery = findAllSQLQuery +
        " WHERE t.event = ? AND t.date = ?";
    private static final String selectCountSQLQuery = "SELECT COUNT(id) FROM tickets";
    private static final String deleteOneSQLQuery = "DELETE FROM tickets WHERE id = ?";
    private static final String saveSQLQuery = "REPLACE INTO tickets " +
        "(id, user, event, price, date, seat) VALUES (?, ?, ?, ?, ?, ?)";
    
    public TicketDAO(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }

    public Ticket findOne(Long key) {        
        return (Ticket)jdbcTemplate.queryForObject(findOneSQLQuery,
                                                   new Object[]{key},
                                                   new TicketRowMapper());
    };

    public Iterable<Ticket> findAll() {
        return jdbcTemplate.query(findAllSQLQuery, new TicketRowMapper());
    };

    public Long count() {
        return jdbcTemplate.queryForObject( selectCountSQLQuery, Long.class );
    };

    public void delete(Ticket ticket) {
        jdbcTemplate.update(deleteOneSQLQuery, ticket.getId());
    };

    public void save(Ticket ticket) {
        jdbcTemplate.update( saveSQLQuery, ticket.getId(), ticket.getUser().getId(),
                             ticket.getEvent().getId(), ticket.getPrice(),
                             ticket.getDate(), ticket.getSeat());
    };

    public boolean exists(Long key) {
        return findOne(key) != null;
    };

    public Iterable<Ticket> searchByEventDate(Event event, Date date) {
        return jdbcTemplate.query(searchByEventDateSQLQuery,
                                  new Object[]{event.getId(), date}, new TicketRowMapper());
    }
}
