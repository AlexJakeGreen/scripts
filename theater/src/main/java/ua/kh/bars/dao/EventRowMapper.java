package ua.kh.bars.dao;

import ua.kh.bars.entity.Event;
import ua.kh.bars.entity.EventRating;
import ua.kh.bars.entity.Auditorium;
import org.springframework.jdbc.core.RowMapper;
import java.sql.SQLException;
import java.sql.ResultSet;

public class EventRowMapper implements RowMapper {
    
    public Event mapRow(ResultSet rs, int rowNum) throws SQLException {
        return new Event(rs.getLong("eventId"),
                         rs.getDate("eventDate"),
                         rs.getString("eventName"),
                         rs.getInt("eventPrice"),
                         EventRating.values()[rs.getInt("eventRating")],
                         new Auditorium( rs.getLong("auditoriumId"),
                                         rs.getString("auditoriumName"),
                                         rs.getInt("auditoriumSeats"),
                                         rs.getString("auditoriumVipSeats")));
    }

}
