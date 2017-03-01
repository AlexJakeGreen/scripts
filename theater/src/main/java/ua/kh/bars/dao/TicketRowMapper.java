package ua.kh.bars.dao;

import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.User;
import ua.kh.bars.entity.Ticket;
import ua.kh.bars.entity.Auditorium;
import ua.kh.bars.entity.EventRating;
import org.springframework.jdbc.core.RowMapper;
import java.sql.SQLException;
import java.sql.ResultSet;

public class TicketRowMapper implements RowMapper {
    
    public Ticket mapRow(ResultSet rs, int rowNum) throws SQLException {
        return new  Ticket(rs.getLong("ticketId"),
                           new User(rs.getLong("userId"),
                                    rs.getString("userEmail"),
                                    rs.getString("userName"),
                                    rs.getDate("userBirthday"),
                                    rs.getString("userSystemMessage")),
                           new Event(rs.getLong("eventId"),
                                     rs.getDate("eventDate"),
                                     rs.getString("eventName"),
                                     rs.getInt("eventPrice"),
                                     EventRating.values()[rs.getInt("eventRating")],
                                     new Auditorium(rs.getLong("auditoriumId"),
                                                    rs.getString("auditoriumName"),
                                                    rs.getInt("auditoriumSeats"),
                                                    rs.getString("auditoriumVipSeats"))),
                           rs.getInt("ticketPrice"),
                           rs.getInt("ticketSeat"),
                           rs.getDate("ticketDate"));
    }

}
